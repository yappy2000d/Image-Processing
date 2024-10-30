import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  base: "/Image-Processing/",
  title: "Image-Processing",
  description: "一個輕量的C++影像處理庫",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: "Home", link: "/" },
      { text: "Docs", link: "/pages/setup" },
      {
        text: "Examples",
        link: "https://github.com/yappy2000d/Image-Processsing/tree/main/example",
      },
    ],

    sidebar: [
      { text: "簡介",link: "/pages/intro" },
      { text: "開始", link: "/pages/startup" },
      {
        text: "類別",
        items: [
          {
            text: "RGBImage",
            link: "/pages/RGBImage",
            collapsed: true,
            items: [{ text: "RGBTRIPLE", link: "/pages/RGBTRIPLE" }],
          },
          { text: "GrayImage", link: "/pages/GrayImage" },
          { text: "Matrix", link: "/pages/Matrix" },
        ],
      },
      {
        text: "函數",
        items: [
          { text: "函數", link: "/pages/functions" },
        ],
      },
      {
        text: "範例",
        link: "https://github.com/yappy2000d/Image-Processsing/tree/main/example",
      },
    ],
  },

  markdown: {
    // https://vitepress.dev/reference/markdown
    math: true,
  },
});
