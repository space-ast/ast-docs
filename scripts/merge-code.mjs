/**
 * 把 Markdown 中分散的代码片段合并成完整源文件。
 *
 * 约定：代码块的 info string 带上 title="<文件名>"，例如
 *
 *     ```cpp title="load_de_eph.cpp"
 *
 * info string 里再加上 indent 表示该片段属于 main() 函数体，例如
 *
 *     ```cpp title="load_de_eph.cpp" indent
 *
 * 这类片段在 md 里顶格书写，拼接时由脚本统一缩进，并自动补上 main() 框架；
 * 没有 indent 的片段按原样放在文件开头（如 #include、using 声明）。
 *
 * 同一个页面里 title 相同的代码块会按出现顺序拼接，生成两份内容相同的文件：
 *
 *   1. md 同目录下的 <文件名>，提交进仓库，页面里的相对链接 [x](x) 可直接命中；
 *   2. .vuepress/public/<页面目录>/<文件名>，dev/build 时由站点对外提供。
 *
 * 第 2 份是构建产物，不要提交。
 *
 * 用法：
 *   node scripts/merge-code.mjs            # 生成 / 更新
 *   node scripts/merge-code.mjs --check    # 只检查是否最新，有差异时退出码 1
 *   node scripts/merge-code.mjs a.md ...   # 只处理指定文件
 */
import {
    existsSync,
    mkdirSync,
    readFileSync,
    readdirSync,
    statSync,
    writeFileSync,
} from "node:fs";
import { dirname, join, relative, resolve, sep } from "node:path";
import { fileURLToPath } from "node:url";

const DOCS_DIR = resolve(dirname(fileURLToPath(import.meta.url)), "..");
const PUBLIC_DIR = join(DOCS_DIR, ".vuepress", "public");
const SCAN_ROOTS = ["zh/examples", "en/examples"];
const UTF8_BOM = String.fromCharCode(0xfeff);

const FENCE_OPEN = /^ {0,3}(`{3,}|~{3,})(.*)$/;
const FENCE_CLOSE = /^ {0,3}(`{3,}|~{3,})[ \t]*$/;
const TITLE_RE = /title\s*=\s*(?:"([^"]+)"|'([^']+)')/;

/** 去掉片段首尾的空行，保留行内缩进。 */
function trimBlankLines(lines) {
    let start = 0;
    let end = lines.length;
    while (start < end && lines[start].trim() === "") start += 1;
    while (end > start && lines[end - 1].trim() === "") end -= 1;
    return lines.slice(start, end);
}

/** 按 title 收集一个 md 里的所有代码块，保持文档顺序。 */
function extractSnippets(mdPath) {
    const snippets = new Map();
    const lines = readFileSync(mdPath, "utf8").split(/\r?\n/);

    let inFence = false;
    let fenceChar = "";
    let fenceLength = 0;
    let title = null;
    let inMain = false;
    let buffer = [];

    for (const line of lines) {
        if (!inFence) {
            const open = FENCE_OPEN.exec(line);
            if (!open) continue;

            inFence = true;
            fenceChar = open[1][0];
            fenceLength = open[1].length;
            const match = TITLE_RE.exec(open[2]);
            title = match ? match[1] ?? match[2] : null;
            inMain = match ? /\b(?:indent|body)\b/.test(open[2].replace(TITLE_RE, "")) : false;
            buffer = [];
            continue;
        }

        const close = FENCE_CLOSE.exec(line);
        if (close && close[1][0] === fenceChar && close[1].length >= fenceLength) {
            if (title) {
                if (!snippets.has(title)) snippets.set(title, []);
                snippets.get(title).push({
                    text: trimBlankLines(buffer).join("\n"),
                    inMain,
                });
            }
            inFence = false;
            title = null;
            inMain = false;
            buffer = [];
            continue;
        }

        buffer.push(line);
    }

    return snippets;
}

function toPosix(p) {
    return p.split(sep).join("/");
}

/** 给每行加统一缩进，空行保持为空。 */
function indent(code, width = 4) {
    const pad = " ".repeat(width);
    return code
        .split("\n")
        .map((line) => (line === "" ? "" : pad + line))
        .join("\n");
}

function buildContent(mdPath, chunks) {
    const source = toPosix(relative(DOCS_DIR, mdPath));
    const isEnglish = source.startsWith("en/");
    const top = chunks.filter((chunk) => !chunk.inMain).map((chunk) => chunk.text);
    const body = chunks.filter((chunk) => chunk.inMain).map((chunk) => chunk.text);

    const lines = [
        isEnglish
            ? `// Full source code for ${source}`
            : `// 本文件为 ${source} 中的完整代码`,
        "",
    ];

    if (top.length) lines.push(top.join("\n\n"), "");

    if (body.length) {
        lines.push("int main()", "{", indent(body.join("\n\n")), "", "    return 0;", "}", "");
    }

    const content = lines.join("\n");

    // 中文页面以 text/plain 直接打开时没有 charset 声明，BOM 让浏览器按 UTF-8 解码；
    // MSVC 也依赖 BOM 识别 UTF-8 源码中的中文字符串。英文页面不需要。
    return isEnglish ? content : `${UTF8_BOM}${content}`;
}

function* walkMarkdown(dir) {
    if (!existsSync(dir)) return;
    for (const name of readdirSync(dir).sort()) {
        const full = join(dir, name);
        if (statSync(full).isDirectory()) {
            yield* walkMarkdown(full);
        } else if (name.endsWith(".md")) {
            yield full;
        }
    }
}

/** 返回该 md 需要写出的所有文件。 */
function collectOutputs(mdPath) {
    const outputs = [];
    for (const [name, chunks] of extractSnippets(mdPath)) {
        if (!name || name.startsWith(".") || /[\\/]/.test(name)) {
            console.warn(`[merge-code] 跳过非法 title "${name}"（${toPosix(relative(DOCS_DIR, mdPath))}）`);
            continue;
        }
        const content = buildContent(mdPath, chunks);
        const dir = dirname(mdPath);
        outputs.push({ file: join(dir, name), content });
        outputs.push({
            file: join(PUBLIC_DIR, relative(DOCS_DIR, dir), name),
            content,
        });
    }
    return outputs;
}

/**
 * 扫描约定目录（或指定文件），生成/校验合并后的源码。
 *
 * @param {{ check?: boolean, files?: string[] }} [options]
 * @returns {{ checked: number, changed: number }}
 */
export function mergeCodeSnippets({ check = false, files } = {}) {
    const mdFiles = files?.length
        ? files.map((file) => resolve(file))
        : SCAN_ROOTS.flatMap((root) => [...walkMarkdown(join(DOCS_DIR, root))]);

    let checked = 0;
    let changed = 0;

    for (const mdPath of mdFiles) {
        if (!existsSync(mdPath)) {
            console.error(`[merge-code] 文件不存在：${mdPath}`);
            changed += 1;
            continue;
        }

        for (const { file, content } of collectOutputs(mdPath)) {
            checked += 1;
            const current = existsSync(file) ? readFileSync(file, "utf8") : null;
            if (current === content) continue;

            changed += 1;
            const label = toPosix(relative(DOCS_DIR, file));
            if (check) {
                console.error(`[merge-code] 已过期：${label}`);
                continue;
            }

            mkdirSync(dirname(file), { recursive: true });
            writeFileSync(file, content, "utf8");
            console.log(`[merge-code] 写入 ${label}`);
        }
    }

    if (check && changed > 0) {
        console.error("[merge-code] 有文件未同步，请运行 npm run merge-code 后重新提交。");
    }

    return { checked, changed };
}

const invokedPath = process.argv[1] ? resolve(process.argv[1]) : "";
if (invokedPath === fileURLToPath(import.meta.url)) {
    const args = process.argv.slice(2);
    const check = args.includes("--check");
    const files = args.filter((arg) => !arg.startsWith("--"));
    const { changed } = mergeCodeSnippets({ check, files });
    process.exit(check && changed > 0 ? 1 : 0);
}
