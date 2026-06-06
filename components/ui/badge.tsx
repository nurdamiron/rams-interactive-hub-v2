/**
 * Badge Component
 * Status indicators and labels with RAMS styling
 */

import * as React from "react";
import { motion, HTMLMotionProps } from "framer-motion";
import { cn } from "@/lib/utils";

export interface BadgeProps extends HTMLMotionProps<"div"> {
  variant?: "default" | "primary" | "success" | "warning" | "info" | "outline";
  children: React.ReactNode;
}

const badgeVariants = {
  default: "bg-surface-dark text-text-light border-border-dark",
  primary: "bg-primary text-charcoal border-primary",
  success: "bg-green-600/20 text-green-400 border-green-600/30",
  warning: "bg-yellow-600/20 text-yellow-400 border-yellow-600/30",
  info: "bg-blue-600/20 text-blue-400 border-blue-600/30",
  outline: "bg-transparent text-text-light border-border-dark",
};

const Badge = React.forwardRef<HTMLDivElement, BadgeProps>(
  ({ className, variant = "default", children, ...props }, ref) => {
    return (
      <motion.div
        ref={ref}
        className={cn(
          "inline-flex items-center rounded-md border px-2.5 py-0.5",
          "text-xs font-semibold tracking-widest uppercase",
          "transition-colors focus:outline-none focus:ring-2 focus:ring-primary focus:ring-offset-2",
          badgeVariants[variant],
          className
        )}
        initial={{ opacity: 0, scale: 0.9 }}
        animate={{ opacity: 1, scale: 1 }}
        transition={{ duration: 0.2 }}
        {...props}
      >
        {children}
      </motion.div>
    );
  }
);

Badge.displayName = "Badge";

export { Badge };
