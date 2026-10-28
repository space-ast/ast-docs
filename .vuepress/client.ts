import { defineClientConfig } from "vuepress/client";

export default defineClientConfig({
  enhance({ siteData }) {
    // "/" locale 仅用于让 vuepress-theme-hope 为 /404.html 生成主题数据
    // 将其从站点 locale 中移除，否则导航栏语言下拉菜单会重复列出中文
    if (siteData?.value.locales) delete siteData.value.locales["/"];
  },
});
