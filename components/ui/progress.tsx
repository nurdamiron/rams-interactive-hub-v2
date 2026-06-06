/**
 * Progress Bar Component
 * Used for location distances, sales progress, etc.
 */

import * as React from "react";
import { motion } from "framer-motion";
import { cn } from "@/lib/utils";

export interface ProgressProps extends React.HTMLAttributes<HTMLDivElement> {
  value: number; // 0-100
  variant?: "default" | "primary" | "success";
  showLabel?: boolean;
  label?: string;
  animated?: boolean;
}

const progressVariants = {
  default: "bg-border-dark",
  primary: "bg-primary",
  success: "bg-green-600",
};

const Progress = React.forwardRef<HTMLDivElement, ProgressProps>(
  (
    {
      className,
      value,
      variant = "primary",
      showLabel = false,
      label,
      animated = true,
      ...props
    },
    ref
  ) => {
    const clampedValue = Math.min(100, Math.max(0, value));

    return (
      <div ref={ref} className={cn("w-full", className)} {...props}>
        {showLabel && label && (
          <div className="flex justify-between text-xs text-text-muted-dark mb-1">
            <span>{label}</span>
            <span>{clampedValue}%</span>
          </div>
        )}
        <div className="relative h-1.5 w-full overflow-hidden rounded-full bg-border-dark/30">
          <motion.div
            className={cn("h-full rounded-full", progressVariants[variant])}
            initial={{ width: 0 }}
            animate={{ width: `${clampedValue}%` }}
            transition={{
              duration: animated ? 1 : 0,
              ease: "easeOut",
              delay: animated ? 0.1 : 0,
            }}
          />
        </div>
      </div>
    );
  }
);

Progress.displayName = "Progress";

export { Progress };
