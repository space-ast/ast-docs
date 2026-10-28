import { navbar } from "vuepress-theme-hope";

export const zhNavbar = navbar([
  {
    icon: "rocket",
    text: "入门指南",
    link: "/zh/guide/",
  },
  {
    icon: "book",
    text: "模块参考",
    link: "/zh/reference/",
  },
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
