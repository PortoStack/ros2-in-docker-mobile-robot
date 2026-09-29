# Medbot Medication Delivery Robot GUI

An autonomous medical delivery robot touchscreen interface and desktop application built with **Next.js 16 (Static Export)**, **Tauri v2**, **Tailwind CSS v4**, **shadcn/ui**, and **SeaORM + SQLite** (Rust Clean Architecture / DDD backend).

Designed for high operational efficiency, clinical safety, touch-first interaction, secure medication transport, real-time ROS 2 local control, hardware serial communication, and central Golang backend telemetry synchronization.

---

## 🏗️ Architecture & Concepts

### 1. 4-Layer Modular Frontend Architecture
```
├── docker/                         # Docker container setups & environment configs
├── tests/                          # Testing suite (Vitest/Jest)
│   ├── unit/                       # Unit test cases
│   └── integration/                # Integration test cases
├── src/
│   ├── app/                        # [Layer 1: Routing] Pages, layouts, & globals.css
│   │   ├── globals.css             # Global styles & Tailwind v4 imports
│   │   └── page.tsx                # Homepage route
│   │
│   ├── core/                       # [Layer 2: App-wide Logic] Core infrastructure & utilities
│   │   ├── api/                    # Base fetcher & HTTP client for platform/api REST
│   │   ├── tauri/                  # Tauri IPC wrappers (invoke helpers)
│   │   ├── auth/                   # Session verification & QR logic
│   │   ├── config/                 # Environment variables validation & global settings
│   │   ├── i18n/                   # Dual-language internationalization engine (th.json / en.json)
│   │   └── store/                  # Global application state stores (Zustand)
│   │
│   ├── features/                   # [Layer 3: Domains] Business domain modules
│   │   ├── medication-delivery/    # Delivery dispatching & prescription orders
│   │   ├── compartments/           # Smart compartment locks & PIN keypads
│   │   ├── telemetry/              # ROS 2 navigation map & telemetry
│   │   ├── rooms/                  # Dynamic rooms & departments management
│   │   └── auth/                   # Authentication views & login forms
│   │
│   └── shared/                     # [Layer 4: Reusable UI & Tools] Generic primitives
│       ├── components/             # Location of Shadcn UI components (Button, Card, Input, Dialog)
│       ├── providers/              # Modular React context providers (AppProvider, QueryProvider)
│       ├── hooks/                  # Generic utility hooks (e.g., useIsMobile)
│       ├── lib/                    # Common utility functions (e.g., date formatting, cn helper)
│       └── schemas/                # Reusable Zod schemas across features
└── src-tauri/                      # Tauri Rust Desktop Backend (Clean Architecture / DDD)
```

### 2. Desktop Client-Driven Strategy (No SSR / No Server Actions)
Next.js App Router is used exclusively as a static routing/UI framework (`output: 'export'`). Server-Side Rendering (SSR) and Server Actions are strictly excluded.
```
Next.js UI (Static Export)
    │
    └── Client Components
             │
             ▼
       TanStack Query
             │
       ┌─────┴─────┐
       │           │
    Tauri IPC    HTTP API
       │           │
    SQLite      platform/api
```

### 3. Rust Native Gateway (Clean Architecture / DDD)
Rust in Tauri acts as the Native Gateway bridging Next.js `invoke()` calls to local hardware, SQLite (SeaORM), serial ports, and ROS 2 nodes:
```
src-tauri/src/
├── commands/               # Tauri IPC Commands exposed to frontend (robot, serial, database, system, room)
├── application/            # Application Services & Use Cases (room_service.rs, delivery_service.rs)
├── domain/                 # Domain Entities & Repository Traits (room.rs, delivery.rs)
└── infrastructure/         # External Implementations
    ├── database/           # SeaORM + SQLite Repositories (room_repository.rs)
    ├── serial/             # Serial port drivers
    └── ros/                # ROS 2 node bridges
```

---

## 🌟 Key Features

- **Autonomous Medication Delivery**: Dispatch prescription orders to dynamic hospital rooms and wards.
- **Smart Compartment Access Control**: PIN/passcode and RFID authentication for secure medication loading and retrieval via hardware serial locks.
- **Dual-Language Localization (TH/EN)**: Native Thai primary UI language with English secondary language toggle (`th.json` / `en.json`).
- **Real-Time Robot Telemetry**:
  - **ROS 2 ↔ Tauri GUI (Local On-Board)**: ROSBridge WebSocket (`ws://localhost:9090`) / Tauri Rust IPC for zero-latency local navigation goals, battery monitoring, and emergency stops.
  - **ROS 2 ↔ Central Backend (MQTT)**: MQTT telemetry (`medbot/robot-01/telemetry`) with QoS 1 message delivery.

---

## 🚀 Getting Started

### Prerequisites

- [Node.js](https://nodejs.org/) (v20+ recommended)
- [Rust](https://www.rust-lang.org/) (1.77.2+)
- Cargo & System Build Dependencies for Tauri v2

### Installation

```bash
npm install
```

### Running Development

```bash
# Start Tauri desktop development environment (Next.js frontend + Rust backend)
npm run tauri dev
```

---

## 📜 Available Scripts

| Command | Description |
| :--- | :--- |
| `npm run tauri dev` | Launch Tauri desktop application in development mode |
| `npm run dev` | Start Next.js development server |
| `npm run build` | Compile Next.js static export bundle (`output: 'export'`) |
| `npm run tauri build` | Build production Tauri desktop release executable |
| `npm run lint` | Run ESLint checks |
| `npm run format` | Format code with Prettier and apply lint fixes |

---

## 📖 Specifications & Guidelines

- **[RULES.md](file:///Users/porto/Workspace/Own/medbot/robot/gui/.agents/RULES.md)**: Static core development, Tailwind CSS v4, Lucide icons, Noto Sans Thai typography.
- **[ARCHITECTURE.md](file:///Users/porto/Workspace/Own/medbot/robot/gui/.agents/ARCHITECTURE.md)**: 4-Layer Modular Frontend Architecture, Tauri IPC Native Gateway, SeaORM + SQLite Clean Architecture / DDD design.
- **[DESIGN.md](file:///Users/porto/Workspace/Own/medbot/robot/gui/.agents/DESIGN.md)**: Clinical design system, touchscreen touch-first guidelines, status badges.
- **[CONTEXT.md](file:///Users/porto/Workspace/Own/medbot/robot/gui/.agents/CONTEXT.md)**: Medication delivery robot domain knowledge & protocols.
- **[WORKFLOW.md](file:///Users/porto/Workspace/Own/medbot/robot/gui/.agents/WORKFLOW.md)**: Development, formatting, and build verification workflow.
