/**
 * Icon Component
 * Wrapper for Lucide React icons with Material Symbols compatibility mapping
 */

import * as React from "react";
import {
  AlertCircle,
  RotateCw,
  Sun,
  Moon,
  X,
  Calendar,
  Building2,
  Layers,
  Building,
  Ruler,
  MapPin,
  FileText,
  Star,
  CheckCircle2,
  Play,
  Pause,
  ArrowLeft,
  PlayCircle,
  ChevronLeft,
  ChevronRight,
  Video,
  Image as ImageIcon,
  Lightbulb,
  History,
  BarChart3,
  Info,
  Volume2,
  Volume1,
  VolumeX,
  Minimize2,
  CircleAlert,
  LucideProps
} from "lucide-react";
import { cn } from "@/lib/utils";

// Mapping from Material Symbol names to Lucide components
const iconMap: Record<string, React.ElementType> = {
  "error_outline": AlertCircle,
  "refresh": RotateCw,
  "light_mode": Sun,
  "dark_mode": Moon,
  "close": X,
  "event": Calendar,
  "business": Building2,
  "layers": Layers,
  "apartment": Building,
  "straighten": Ruler,
  "location_on": MapPin,
  "article": FileText,
  "stars": Star,
  "check_circle": CheckCircle2,
  "play_arrow": Play,
  "arrow_back": ArrowLeft,
  "play_circle": PlayCircle,
  "chevron_left": ChevronLeft,
  "chevron_right": ChevronRight,
  "videocam": Video,
  "photo": ImageIcon,
  "lightbulb": Lightbulb,
  "history": History,
  "analytics": BarChart3,
  "info": Info,
  "pause": Pause,
  "volume_up": Volume2,
  "volume_down": Volume1,
  "volume_off": VolumeX,
  "fullscreen_exit": Minimize2,
  "error": CircleAlert,
};

export interface IconProps extends Omit<LucideProps, 'size'> {
  name: string;
  size?: "sm" | "md" | "lg" | "xl" | "2xl" | "3xl" | "4xl" | "5xl";
  filled?: boolean; // Note: Lucide icons don't support "filled" in the same way, but we can use fill="currentColor"
}

const sizeValueMap = {
  sm: 16,
  md: 20,
  lg: 24,
  xl: 36,
  "2xl": 48,
  "3xl": 60,
  "4xl": 72,
  "5xl": 96,
};

const Icon = React.forwardRef<SVGSVGElement, IconProps>(
  ({ className, name, size = "md", filled = false, ...props }, ref) => {
    const LucideIcon = iconMap[name];

    if (!LucideIcon) {
      console.warn(`Icon "${name}" not found in mapping`);
      return <span className={className}>{name}</span>;
    }

    return (
      <LucideIcon
        ref={ref}
        size={sizeValueMap[size]}
        className={cn("shrink-0", className)}
        {...(filled ? { fill: "currentColor" } : {})}
        {...props}
      />
    );
  }
);

Icon.displayName = "Icon";

export { Icon };
