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
        destination: 'http://RAMS-ESP32.local/api/:path*', // Proxy to ESP32 via mDNS
      },
    ];
  },
};

export default nextConfig;
