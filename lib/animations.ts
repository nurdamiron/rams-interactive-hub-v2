/**
 * Advanced Animation Variants & Utilities
 * Based on 2025 Best Practices
 */

import { Variants } from "framer-motion";

// Easing functions (luxury feel)
export const easings = {
  smooth: [0.16, 1, 0.3, 1] as const, // Smooth, elegant easing
  snappy: [0.34, 1.56, 0.64, 1] as const, // Snappy with slight bounce
  gentle: [0.25, 0.1, 0.25, 1] as const, // Gentle, subtle
  elastic: [0.68, -0.55, 0.265, 1.55] as const, // Elastic bounce
};

// Fade animations
export const fadeInUp: Variants = {
  hidden: {
    opacity: 0,
    y: 40,
    filter: "blur(4px)"
  },
  visible: {
    opacity: 1,
    y: 0,
    filter: "blur(0px)",
    transition: {
      duration: 0.6,
      ease: easings.smooth,
    }
  },
};

export const fadeIn: Variants = {
  hidden: { opacity: 0 },
  visible: {
    opacity: 1,
    transition: {
      duration: 0.5,
      ease: "easeOut",
    }
  },
};

// Stagger container for lists
export const staggerContainer: Variants = {
  hidden: { opacity: 0 },
  visible: {
    opacity: 1,
    transition: {
      staggerChildren: 0.1,
      delayChildren: 0.2,
    },
  },
};

// Stagger item
export const staggerItem: Variants = {
  hidden: {
    opacity: 0,
    y: 20,
    scale: 0.95,
  },
  visible: {
    opacity: 1,
    y: 0,
    scale: 1,
    transition: {
      duration: 0.5,
      ease: easings.smooth,
    }
  },
};

// Scale animations
export const scaleIn: Variants = {
  hidden: { scale: 0.8, opacity: 0 },
  visible: {
    scale: 1,
    opacity: 1,
    transition: {
      duration: 0.5,
      ease: easings.elastic,
    }
  },
};

// Slide animations
export const slideInLeft: Variants = {
  hidden: { x: -100, opacity: 0 },
  visible: {
    x: 0,
    opacity: 1,
    transition: {
      duration: 0.6,
      ease: easings.smooth,
    }
  },
};

export const slideInRight: Variants = {
  hidden: { x: 100, opacity: 0 },
  visible: {
    x: 0,
    opacity: 1,
    transition: {
      duration: 0.6,
      ease: easings.smooth,
    }
  },
};

// Luxury card hover
export const luxuryCardHover = {
  rest: {
    scale: 1,
    y: 0,
    boxShadow: "0 4px 20px -2px rgba(0, 0, 0, 0.1)",
  },
  hover: {
    scale: 1.02,
    y: -8,
    boxShadow: "0 20px 40px -8px rgba(0, 0, 0, 0.3)",
    transition: {
      duration: 0.3,
      ease: easings.smooth,
    }
  },
  tap: {
    scale: 0.98,
    transition: {
      duration: 0.1,
    }
  },
};

// Image zoom on hover
export const imageZoom = {
  rest: { scale: 1 },
  hover: {
    scale: 1.08,
    transition: {
      duration: 0.6,
      ease: easings.gentle,
    }
  },
};

// Rotate animations
export const rotate: Variants = {
  initial: { rotate: 0 },
  animate: {
    rotate: 360,
    transition: {
      duration: 20,
      repeat: Infinity,
      ease: "linear",
    }
  },
};

// Pulse animation
export const pulse: Variants = {
  initial: { scale: 1, opacity: 0.8 },
  animate: {
    scale: [1, 1.1, 1],
    opacity: [0.8, 1, 0.8],
    transition: {
      duration: 2,
      repeat: Infinity,
      ease: "easeInOut",
    }
  },
};

// Shimmer effect
export const shimmer: Variants = {
  initial: { x: "-100%" },
  animate: {
    x: "200%",
    transition: {
      duration: 2,
      repeat: Infinity,
      ease: "linear",
      repeatDelay: 1,
    }
  },
};

// Page transitions
export const pageTransition: Variants = {
  initial: {
    opacity: 0,
    scale: 0.95,
    filter: "blur(10px)",
  },
  animate: {
    opacity: 1,
    scale: 1,
    filter: "blur(0px)",
    transition: {
      duration: 0.5,
      ease: easings.smooth,
    }
  },
  exit: {
    opacity: 0,
    scale: 1.05,
    filter: "blur(10px)",
    transition: {
      duration: 0.3,
      ease: "easeIn",
    }
  },
};

// Magnetic hover effect (follows cursor)
export const magneticHover = (strength: number = 0.3) => ({
  rest: { x: 0, y: 0 },
  hover: (offset: { x: number; y: number }) => ({
    x: offset.x * strength,
    y: offset.y * strength,
    transition: {
      type: "spring",
      stiffness: 300,
      damping: 20,
    },
  }),
});

// Scroll reveal (use with useInView)
export const scrollReveal: Variants = {
  hidden: {
    opacity: 0,
    y: 75,
    scale: 0.9,
  },
  visible: {
    opacity: 1,
    y: 0,
    scale: 1,
    transition: {
      duration: 0.8,
      ease: easings.smooth,
    }
  },
};

// Micro-interactions
export const buttonTap = {
  scale: 0.95,
  transition: {
    type: "spring",
    stiffness: 400,
    damping: 17,
  },
};

export const iconSpin = {
  rotate: [0, 360],
  transition: {
    duration: 0.6,
    ease: "easeInOut",
  },
};

// Loading animations
export const loadingBar: Variants = {
  initial: { width: "0%" },
  animate: {
    width: "100%",
    transition: {
      duration: 4,
      ease: "linear",
    }
  },
};

export const loadingDots: Variants = {
  initial: { y: 0 },
  animate: {
    y: [0, -10, 0],
    transition: {
      duration: 0.6,
      repeat: Infinity,
      ease: "easeInOut",
    }
  },
};

// Utility function for stagger delay
export const getStaggerDelay = (index: number, baseDelay: number = 0.1) => ({
  delay: index * baseDelay,
});

// Utility for spring animation
export const springTransition = (
  stiffness: number = 300,
  damping: number = 30
) => ({
  type: "spring" as const,
  stiffness,
  damping,
});
