import Link from "next/link";
import { Smile, Bot, Cpu } from "lucide-react";

export default function Home() {
  return (
    <div className="flex flex-col min-h-screen items-center justify-center bg-background font-sans p-6">
      <main className="flex w-full max-w-2xl flex-col items-center justify-center p-8 bg-card border border-border rounded-3xl shadow-lg text-center space-y-8">
        
        {/* Header Title */}
        <div className="space-y-3">
          <div className="inline-flex items-center gap-2 px-3 py-1 rounded-full bg-primary/10 text-primary text-xs font-mono font-medium">
            <Cpu className="w-3.5 h-3.5" />
            <span>MEDBOT ROBOT GUI</span>
          </div>
          <h1 className="text-3xl font-semibold text-foreground tracking-tight">
            ระบบควบคุมหุ่นยนต์ส่งยา
          </h1>
          <p className="text-muted-foreground text-sm max-w-md mx-auto">
            ยินดีต้อนรับสู่ระบบอินเทอร์เฟซควบคุมและแสดงผลหุ่นยนต์ทางการแพทย์
          </p>
        </div>

        {/* Navigation Action Buttons */}
        <div className="flex flex-col sm:flex-row items-center justify-center gap-4 w-full max-w-md">
          {/* Direct Navigation Link to Robot Face */}
          <Link
            href="/face"
            className="flex items-center justify-center gap-3 w-full sm:w-auto px-6 py-4 rounded-2xl bg-primary text-primary-foreground font-medium text-base hover:opacity-90 transition-all shadow-md active:scale-95"
          >
            <Smile className="w-6 h-6" />
            <span>แสดงอารมณ์หุ่นยนต์ (Robot Face)</span>
          </Link>
        </div>

        {/* System Status Footer */}
        <div className="pt-4 border-t border-border w-full flex items-center justify-between text-xs text-muted-foreground font-mono">
          <span className="flex items-center gap-1.5">
            <Bot className="w-4 h-4 text-emerald-500" />
            Robot ID: MEDBOT-01
          </span>
          <span>TAURI v2 + NEXT.JS</span>
        </div>
      </main>
    </div>
  );
}
