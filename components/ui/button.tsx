/**
 * Button Component with Framer Motion animations
 * Based on shadcn/ui but customized for RAMS aesthetic
 */

import * as React from "react";
import { motion, HTMLMotionProps } from "framer-motion";
import { cn } from "@/lib/utils";
import { springTransition } from "@/lib/animations";

export interface ButtonProps extends HTMLMotionProps<"button"> {
  variant?: "primary" | "secondary" | "ghost" | "outline" | "link";
  size?: "sm" | "md" | "lg" | "icon";
  children: React.ReactNode;
}

const buttonVariants = {
  primary:
    "bg-primary hover:bg-primary-hover text-charcoal font-bold tracking-wider uppercase shadow-md hover:shadow-lg",
  secondary:
    "bg-surface-dark hover:bg-charcoal text-text-light border border-border-dark hover:border-primary/50",
  ghost:
    "hover:bg-surface-dark/50 text-text-light hover:text-primary transition-colors",
  outline:
    "border border-border-dark hover:border-primary hover:bg-surface-dark/50 text-text-light hover:text-primary",
  link: "text-primary underline-offset-4 hover:underline hover:text-primary-hover",
};

const sizeVariants = {
  sm: "px-3 py-1.5 text-xs",
  md: "px-6 py-3 text-sm",
  lg: "px-8 py-4 text-base",
  icon: "w-10 h-10 p-0 flex items-center justify-center",
};

const Button = React.forwardRef<HTMLButtonElement, ButtonProps>(
  (
    {
      className,
      variant = "primary",
      size = "md",
      children,
      disabled,
      ...props
    },
    ref
  ) => {
    return (
      <motion.button
        ref={ref}
        className={cn(
          // Base styles
          "inline-flex items-center justify-center rounded-md",
          "font-medium transition-all duration-300",
          "focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-primary focus-visible:ring-offset-2 focus-visible:ring-offset-background-dark",
          "disabled:pointer-events-none disabled:opacity-50",
          // Variant styles
          buttonVariants[variant],
          // Size styles
          sizeVariants[size],
          className
        )}
        disabled={disabled}
        whileHover={{ scale: disabled ? 1 : 1.03, y: disabled ? 0 : -2 }}
        whileTap={{ scale: disabled ? 1 : 0.97 }}
        transition={springTransition(400, 17)}
        {...props}
      >
        {children}
      </motion.button>
    );
  }
);

Button.displayName = "Button";

export { Button };
