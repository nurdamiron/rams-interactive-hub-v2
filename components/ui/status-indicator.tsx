/**
 * Status Indicator Component
 * Shows connection status, online/offline states, etc.
 */

import * as React from "react";
import { motion } from "framer-motion";
import { cn } from "@/lib/utils";

export interface StatusIndicatorProps extends React.HTMLAttributes<HTMLDivElement> {
  status: "online" | "offline" | "warning";
  label?: string;
  showDot?: boolean;
  showPulse?: boolean;
  showLabel?: boolean;
}

const statusConfig = {
  online: {
    color: "bg-green-500",
    textColor: "text-green-400",
    label: "Connected",
  },
  offline: {
    color: "bg-gray-500",
    textColor: "text-gray-400",
    label: "Disconnected",
  },
  warning: {
    color: "bg-yellow-500",
    textColor: "text-yellow-400",
    label: "Warning",
  },
};

const StatusIndicator = React.forwardRef<HTMLDivElement, StatusIndicatorProps>(
  (
    {
      className,
      status = "offline",
      label,
      showDot = true,
      showPulse = true,
      showLabel = true,
      ...props
    },
    ref
  ) => {
    const config = statusConfig[status];
    const displayLabel = label || config.label;

    return (
      <div
        ref={ref}
        className={cn("flex items-center gap-2", className)}
        {...props}
      >
        {showDot && (
          <div className="relative flex h-2 w-2">
            {showPulse && status === "online" && (
              <motion.span
                className={cn(
                  "absolute inline-flex h-full w-full rounded-full opacity-75",
                  config.color
                )}
                animate={{
                  scale: [1, 1.5, 1],
                  opacity: [0.75, 0, 0.75],
                }}
                transition={{
                  duration: 2,
                  repeat: Infinity,
                  ease: "easeInOut",
                }}
              />
            )}
            <span
              className={cn(
                "relative inline-flex rounded-full h-2 w-2",
                config.color
              )}
            />
          </div>
        )}
        {showLabel && displayLabel && (
          <span className={cn("text-xs font-medium", config.textColor)}>
            {displayLabel}
          </span>
        )}
      </div>
    );
  }
);

StatusIndicator.displayName = "StatusIndicator";

export { StatusIndicator };
