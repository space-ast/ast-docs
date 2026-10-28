import { navbar } from "vuepress-theme-hope";

export const zhNavbar = navbar([
  {
    icon: "signs-post",
    text: "使用指南",
    link: "/zh/guide/quickstart.html",
  },
  "/zh/technical/",
  "/zh/examples/",
  {
    icon: "gears",
    text: "API 手册",
    link: "https://space-ast.github.io/ast/api/index.html",
  },
  // "/dev/",
  {
    icon: "blog",
    text: "博客",
    link: "https://space-ast.github.io/ast-blog",
  },
]);
