"use client";

import React, { Component, ErrorInfo, ReactNode } from "react";
import { Button } from "@/components/ui/button";
import { Icon } from "@/components/ui/icon";
import { useLanguage } from "@/lib/i18n";

interface Props {
    children: ReactNode;
    fallback?: ReactNode;
}

interface State {
    hasError: boolean;
    error?: Error;
}

// Localized error UI component (functional to use hooks)
const LocalizedErrorUI = ({ onReload }: { onReload: () => void }) => {
    const { t } = useLanguage();

    return (
        <div className="min-h-screen w-full flex flex-col items-center justify-center bg-zinc-950 text-white p-8">
            <div className="w-24 h-24 rounded-full bg-red-500/10 flex items-center justify-center mb-6">
                <Icon name="error_outline" size="4xl" className="text-red-500" />
            </div>
            <h1 className="text-3xl font-bold mb-4">{t("somethingWentWrong")}</h1>
            <p className="text-zinc-400 mb-8 max-w-md text-center">
                {t("systemErrorMessage")}
            </p>
            <div className="flex gap-4">
                <Button onClick={onReload} variant="primary" size="lg">
                    <Icon name="refresh" className="mr-2" />
                    {t("reloadSystem")}
                </Button>
            </div>

            {/* Auto-reload timer */}
            <AutoReload onReload={onReload} timeout={30000} />
        </div>
    );
};

export class ErrorBoundary extends Component<Props, State> {
    public state: State = {
        hasError: false,
    };

    public static getDerivedStateFromError(error: Error): State {
        return { hasError: true, error };
    }

    public componentDidCatch(error: Error, errorInfo: ErrorInfo) {
        console.error("Uncaught error:", error, errorInfo);
    }

    private handleReload = () => {
        window.location.reload();
    };

    public render() {
        if (this.state.hasError) {
            if (this.props.fallback) {
                return this.props.fallback;
            }

            return <LocalizedErrorUI onReload={this.handleReload} />;
        }

        return this.props.children;
    }
}

// Helper component for auto-reload
const AutoReload = ({ onReload, timeout }: { onReload: () => void, timeout: number }) => {
    React.useEffect(() => {
        const timer = setTimeout(onReload, timeout);
        return () => clearTimeout(timer);
    }, [onReload, timeout]);
    return null;
}
