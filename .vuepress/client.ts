import { defineClientConfig, useRoute } from "vuepress/client";
import { nextTick, onMounted, watch } from "vue";

// Vercount：不蒜子的替代品，兼容 busuanzi_* 标签
const VERCOUNT_SRC = "https://events.vercount.one/js";

// 脚本每次执行都会为当前页面记一次 PV
// 并把结果写入页面里的 vercount_value_site_pv / vercount_value_page_pv 元素
function refreshVercount() {
  if (typeof window === "undefined") return;

  document.getElementById("vercount-script")?.remove();
  const script = document.createElement("script");
  script.id = "vercount-script";
  script.src = VERCOUNT_SRC;
  script.async = true;
  script.referrerPolicy = "no-referrer-when-downgrade";
  document.head.appendChild(script);
}

export default defineClientConfig({
  enhance({ siteData }) {
    // "/" locale 仅用于让 vuepress-theme-hope 为 /404.html 生成主题数据
    // 将其从站点 locale 中移除，否则导航栏语言下拉菜单会重复列出中文
    if (siteData?.value.locales) delete siteData.value.locales["/"];
  },

  setup() {
    const route = useRoute();

    // 挂载后再加载，确保页脚里的计数元素已经渲染出来
    onMounted(() => refreshVercount());

    // SPA 路由切换不会重新执行脚本，页脚会停留在上一页的计数
    watch(
      () => route.path,
      () => nextTick(() => refreshVercount()),
    );
  },
});
