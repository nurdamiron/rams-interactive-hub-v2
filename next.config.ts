import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  // Remove "output: export" to enable server-side features (API routes)
  // output: "export",
  // distDir: "out",
  images: {
    unoptimized: true,
  },
  trailingSlash: true,

  // Proxy ESP32 requests through Next.js server to avoid CORS
  async rewrites() {
    return [
      {
        source: '/esp32-api/:path*',
        destination: 'http://192.168.110.65/api/:path*', // Proxy to ESP32 (STA network)
      },
    ];
  },
};

export default nextConfig;
