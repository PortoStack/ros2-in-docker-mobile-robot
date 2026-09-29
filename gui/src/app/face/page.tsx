'use client';

import React from 'react';
import { RobotFace, useFaceMotionLoop } from '@/features/robot-face';

export default function FacePage() {
  const {
    currentConfig,
    triggerEmotion,
  } = useFaceMotionLoop();

  return (
    <main className="relative w-vw h-vh fixed inset-0 overflow-hidden bg-background flex items-center justify-center p-0">
      {/* Pure Fullscreen Animated Vector Robot Face (No Control Overlay Bar) */}
      <RobotFace
        config={currentConfig}
        onClick={() => triggerEmotion('happy', 3500)}
        className="w-full h-full"
      />

      {/* Subtle Minimal Status Pill at Bottom */}
      <div className="absolute bottom-5 inset-x-0 flex justify-center pointer-events-none z-40">
        <div className="px-6 py-2 rounded-full bg-card/75 backdrop-blur-md border border-border/50 text-foreground/80 text-xs font-mono font-medium tracking-widest uppercase shadow-sm transition-all duration-700">
          {currentConfig.label}
        </div>
      </div>
    </main>
  );
}
