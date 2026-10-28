import { defineUserConfig } from "vuepress";
import { getDirname, path } from 'vuepress/utils'
import theme from "./theme.js";
import { mergeCodeSnippets } from "../scripts/merge-code.mjs";

const __dirname = getDirname(import.meta.url)

export default defineUserConfig({
  base: "/ast/",
  lang:"zh-CN",

  locales: {
    // 根 locale 用于让 vuepress-theme-hope 生成 /en、/zh 之外页面（如 404.html）的主题数据
    // 客户端会将其从站点 locale 列表中移除，避免语言下拉菜单重复显示。
    "/": {
      lang: "zh-CN"
    },
    "/en/": {
      lang: "en-US",
      title: "Aerospace Simulation Algorithm",
      // description: "docs for ast project",
    },
    "/zh/": {
      lang: "zh-CN",
      title: "航天仿真算法库",
      // description: "SpaceAST 项目的文档主页",
    },
  },

  theme,

  // 把 md 中带 title="xxx.cpp" 的代码片段合并成完整源文件
  onPrepared() {
    mergeCodeSnippets();
  },

  markdown:{
    importCode:{
      handleImportPath:  (str: string) => 
        str
        .replace(/^\/examples/, path.resolve(__dirname, '../../examples'))
        .replace(/^\/src/, path.resolve(__dirname, '../../src'))
      ,
    }
  }

  // Enable it with pwa
  // shouldPrefetch: false,
});
