# Maryn Launcher
[Official Maryn Launcher website](https://dinankakoirala.github.io/Maryn_Launcher/)

A lightweight, cross-platform Minecraft Java Edition launcher built with C++ and Qt 6 Widgets. Supports both offline (cracked) and Microsoft-authenticated players, instance-based profile management, Modrinth mod integration, and automatic Java management.

## Why

Most Minecraft launchers are Electron-based and consume 250+ MB of RAM at idle. Maryn uses native Qt Widgets and targets 30–50 MB — roughly 5× lighter.

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Language | C++17 |
| UI Framework | Qt 6 Widgets |
| Build System | CMake |
| HTTP / Downloads | QNetworkAccessManager |
| JSON Parsing | QJsonDocument |
| Process Launch | QProcess |
| Hashing | QCryptographicHash (MD5 for offline UUID) |

## Features

### Authentication
- **Offline / cracked** — fully working. UUID v3 generated from `MD5("OfflinePlayer:" + username)`, compatible with vanilla offline servers
- **Microsoft OAuth** — not yet implemented (planned: full Xbox Live / XSTS / Minecraft auth chain)
- Optional launcher account for skin/cape system *(planned)*

### Instance Management
- Modrinth/Prism-style model — each profile is self-contained
- Shared assets and libraries pool across instances (saves disk space)
- Per-instance settings: MC version, mod loader, Java args, resolution, RAM
- Instance import/export via Modrinth `.mrpack` format
- Instance duplication

### Mod Support *(not yet implemented)*
- Modrinth API integration — search and download mods
- Fabric, Forge, Quilt, NeoForge installer support
- Mod update checker
- One-click Performance Pack: Fabric + Sodium + Lithium + Iris

### Java Management
- Auto-detects system Java version
- Auto-downloads the correct JRE from Adoptium if missing
- Supports Java 8 (MC 1.16−), Java 17 (MC 1.18+), Java 21 (MC 1.20.5+)
- Per-instance Java version stored in `instance.json`

### Skin System
- Custom skin server via JVM flag override
- Cracked players can upload and see each other's skins
- No launcher account required to play offline

### Performance & UX
- Optimized JVM flags preset (G1GC, ParallelRefProcEnabled, etc.)
- Per-instance RAM allocation slider
- Crash detector — parses `latest.log` and surfaces errors in UI
- Offline mode — launches with cached files if no internet
- Screenshot gallery per instance
- Launcher auto-updater
- CLI frontend *(planned — core has zero UI dependencies)*

## Project Structure

```
Maryn_Launcher/
├── CMakeLists.txt
├── main.cpp
└── src/
    ├── core/
    │   ├── auth/
    │   │   ├── AuthProvider.hpp        ← interface
    │   │   ├── OfflineAuthProvider     ← offline/cracked
    │   │   └── MicrosoftAuthProvider   ← planned
    │   ├── instance/
    │   │   ├── Instance                ← one profile
    │   │   └── InstanceManager         ← CRUD
    │   ├── download/
    │   │   ├── DownloadManager         ← QNetworkAccessManager wrapper
    │   │   ├── VersionManifest         ← Mojang version JSON
    │   │   └── AssetManager            ← assets + libraries
    │   ├── launch/
    │   │   └── LaunchEngine            ← builds + spawns java command
    │   ├── skin/
    │   │   ├── SkinProvider.hpp        ← interface
    │   │   ├── LocalSkinProvider
    │   │   └── RemoteSkinProvider      ← planned
    │   └── java/
    │       └── JavaManager             ← detect + download JRE
    └── ui/
        ├── MainWindow
        ├── InstanceGrid                ← profile cards
        ├── AccountPanel
        └── SettingsDialog
```

Core has **zero UI dependencies** — this is intentional and enables the future CLI frontend.

## Storage Layout

```
~/.local/share/MarynLauncher/     (Linux)
%APPDATA%/MarynLauncher/          (Windows)
├── instances/
│   └── <name>/
│       ├── instance.json         ← version, loader, java args
│       └── .minecraft/           ← saves, mods, configs
├── assets/                       ← shared across instances
├── libraries/                    ← shared across instances
├── runtimes/
│   ├── java-8/
│   ├── java-17/
│   └── java-21/
└── accounts.json
```

## External APIs

| API | Purpose |
|-----|---------|
| Mojang Version Manifest | List all MC versions |
| Mojang Session Server | Online auth verification |
| Modrinth API | Search and download mods |
| Adoptium API | Download JRE |
| Microsoft OAuth | Legit account auth |

## Build Gameplan

### Phase 1 — Basic Launch (Weeks 1–4)
- CMake + Qt setup, fetch and parse Mojang version manifest
- Download game jar, libraries, and assets
- Offline UUID generation, construct Java launch command, QProcess launch
- Basic UI — window with instance list and Play button

### Phase 2 — Instance System (Weeks 5–7)
- `Instance` class with `instance.json` read/write
- `InstanceManager` — create, delete, list
- Instance grid UI (Modrinth-style cards)
- Per-instance settings and finalized storage layout

### Phase 3 — Java Manager (Week 8)
- Detect system Java via QProcess
- Map MC version → required Java (8/17/21)
- Auto-download from Adoptium if missing

### Phase 4 — Mod Support (Weeks 9–12)
- Modrinth API: search, version filtering by MC version + loader
- Fabric installer; Forge installer (after Fabric)
- Mod list UI per instance
- One-click Performance Pack

### Phase 5 — Auth + Skins (Weeks 13–14)
- Launcher account groundwork
- Custom skin server JVM flag override
- Local skin provider
- Microsoft OAuth *(optional, add last)*

### Phase 6 — Polish (Weeks 15–18)
- Crash detector
- Offline mode detection
- Launcher auto-updater
- Windows installer (Inno Setup) and Linux AppImage
- Screenshot gallery
- Global settings dialog

## Technical Notes

**Offline UUID generation** — `UUID = MD5("OfflinePlayer:" + username)` with version 3 and variant bits set. Same formula used by vanilla Minecraft, TLauncher, and Prism — ensures cross-launcher compatibility on offline servers.

**Launch command structure:**
```
java -Xms512m -Xmx4096m
     -XX:+UseG1GC -XX:+ParallelRefProcEnabled
     -Djava.library.path=<natives>
     -cp <libs>:<game_jar>
     net.minecraft.client.main.Main
     --username <name> --uuid <uuid>
     --accessToken 0 --userType legacy
     --version <mc_version>
     --gameDir <instance_dir>
     --assetsDir <assets_dir>
     --assetIndex <index>
```

**Skin server override** — redirect skin fetching via JVM flags:
```
-Dminecraft.api.auth.host=https://yourserver.com
-Dminecraft.api.session.host=https://yourserver.com
-Dminecraft.api.services.host=https://yourserver.com
```

## Current State

- Qt 6.11.1 installed, Qt Creator configured
- CMake project created (`Maryn_Launcher`)
- Empty Qt Widgets window compiles and runs
- GitHub repo initialized at `repos/Maryn_Launcher/`
- **Next:** create `src/` folder structure, move mainwindow files to `src/ui/`, update `CMakeLists.txt`
