import { navbar } from "vuepress-theme-hope";

export const zhNavbar = navbar([
  "/zh/guide/",
  "/zh/reference/",
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
