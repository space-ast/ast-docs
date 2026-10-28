import { navbar } from "vuepress-theme-hope";

export const enNavbar = navbar([
  "/en/guide/",
  "/en/reference/",
  "/en/examples/",
  {
    icon: "gears",
    text: "API docs",
    link: "https://space-ast.github.io/ast/api/index.html",
  },
  {
    icon: "blog",
    text: "Blog",
    link: "https://space-ast.github.io/ast-blog",
  },
]);
