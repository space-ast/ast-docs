import { navbar } from "vuepress-theme-hope";

export const enNavbar = navbar([
  "/en/",
  {
    icon: "rocket",
    text: "Guide",
    link: "/en/guide/",
  },
  {
    icon: "book",
    text: "Module reference",
    link: "/en/reference/",
  },
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
