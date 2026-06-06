import type { Config } from "tailwindcss";

const config: Config = {
  content: [
    "./pages/**/*.{js,ts,jsx,tsx,mdx}",
    "./components/**/*.{js,ts,jsx,tsx,mdx}",
    "./app/**/*.{js,ts,jsx,tsx,mdx}",
  ],
  theme: {
    extend: {
      colors: {
        primary: {
          DEFAULT: "#c49f64",
          hover: "#b08d55",
          dark: "#a27b5c",
        },
        "primary-hover": "#b08d55",
        "primary-dark": "#a27b5c",
        background: {
          light: "#f8f7f6",
          dark: "#161513",
        },
        "background-light": "#f8f7f6",
        "background-dark": "#161513",
        surface: {
          light: "#ffffff",
          dark: "#1f1d1a",
        },
        "surface-light": "#ffffff",
        "surface-dark": "#1f1d1a",
        border: {
          light: "#e8e2d6",
          dark: "#35312c",
          subtle: "#35312c",
        },
        "border-light": "#e8e2d6",
        "border-dark": "#35312c",
        "border-subtle": "#35312c",
        text: {
          dark: "#2a2723",
          light: "#f8f7f6",
          muted: "#6b665f",
          "muted-dark": "#b2ada3",
        },
        "text-dark": "#2a2723",
        "text-light": "#f8f7f6",
        "text-muted": "#6b665f",
        "text-muted-dark": "#b2ada3",
        charcoal: "#1e1a14",
        "neutral-warm": "#f4f1ea",
      },
      fontFamily: {
        display: ["Manrope", "ui-sans-serif", "system-ui", "sans-serif"],
        body: ["Manrope", "ui-sans-serif", "system-ui", "sans-serif"],
      },
    },
  },
  plugins: [],
};

export default config;
