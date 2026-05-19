# ⚡ NEUTRON

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square&logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-green?style=flat-square&logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)](LICENSE)
[![500+ Formats](https://img.shields.io/badge/File%20Formats-500%2B-orange?style=flat-square)](neutron.conf)
[![Built While Learning](https://img.shields.io/badge/Built%20While-Learning%20C%2B%2B-purple?style=flat-square)](https://shields.io)
[![Downloads Chaos: Defeated](https://img.shields.io/badge/Downloads%20Chaos-Defeated-red?style=flat-square)](https://shields.io)

> A C++ file organizer that sorts your chaotic Downloads folder so you don't have to.

---

## 🤔 What Is This?

NEUTRON scans a folder, reads your sorting rules from a config file, and moves each file into the right subfolder based on its extension. That's it. No cloud, no subscriptions, no electron app eating 400MB of RAM. Just a binary, a config, and a slightly less messy Downloads folder.

**500+ file formats recognized.** `.nes` ROMs, `.qcow2` VM images, `.fastq` DNA sequences — it's all in there. Probably overkill. Absolutely worth it.

---

## 📁 Before / After

**Before NEUTRON:**
```
Downloads/
├── invoice_final_FINAL_v3.pdf
├── funny_cat.mp4
├── ubuntu-22.04.iso
├── backup_old.zip
├── mycoolproject.cpp
├── song_i_illegally_downloaded.mp3
├── definitely_not_a_virus.exe
└── (537 more files you haven't touched since 2021)
```

**After NEUTRON:**
```
Downloads/
├── Documents/
│   └── invoice_final_FINAL_v3.pdf
├── Video/
│   └── funny_cat.mp4
├── Archives/
│   └── ubuntu-22.04.iso
├── Backups/
│   └── backup_old.zip
├── Code/
│   └── mycoolproject.cpp
├── Audio/
│   └── song_i_illegally_downloaded.mp3
├── Programs/
│   └── definitely_not_a_virus.exe
└── Misc/
    └── (everything NEUTRON didn't recognize)
```

Peace. Finally.

---

## 🚀 Quick Start

### 1. Clone it
```bash
git clone https://github.com/yourusername/neutron.git
cd neutron
```

### 2. Configure it

Open `neutron.conf` and set your source folder:

```ini
# Windows
source = C:/Users/YourName/Downloads/

# Linux / macOS
source = /home/yourname/Downloads/
```

That's the only required change. Everything else has sensible defaults.

### 3. Build it

**Linux / macOS:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**Windows (Visual Studio):**
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

**Windows (MinGW):**
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Requirements: CMake 3.15+, a C++20-capable compiler (GCC 10+, Clang 10+, MSVC 2019+)

### 4. Run it

Put `neutron.conf` in the same directory as the binary, then:

```bash
# Linux / macOS
./neutron

# Windows
neutron.exe
```

You'll see something like:
```
=== NEUTRON File Organizer ===
Source: C:/Users/YourName/Downloads/
Rules: 487 (hopefully enough)

[OK] invoice_final_FINAL_v3.pdf → Documents/
[OK] funny_cat.mp4 → Video/
[SKIP] ubuntu-22.04.iso (already there, not touching it)
...

=== SUMMARY ===
Total:      540
Organized:  521
Skipped:    15
Errors:     4

NEUTRON out. Files organized. Chaos defeated.
```

---

## ⚙️ Config Customization

`neutron.conf` is just a text file. Format: `extension = FolderName`

```ini
# Comments start with #
# Spaces around '=' are fine

source = /home/you/Downloads/

# Built-in defaults (already included in the full config)
.pdf    = Documents
.mp4    = Video
.mp3    = Audio
.zip    = Archives

# Add your own rules
.blend  = Blender
.sketch = DesignAssets
.log    = Logs
.sql    = DatabaseDumps

# Unknown extensions go to Misc/ automatically
```

**Rules:**
- Extensions must start with `.` and are case-insensitive (`.JPG` = `.jpg`)
- Folders are created automatically if they don't exist
- Files that already exist at the destination are **skipped, not overwritten**
- Anything not matched goes to `Misc/`

---

## ⚠️ Safety Warnings

**NEUTRON moves files, it does not copy them.** This is intentional (fast, no disk doubling) but means:

- 🔴 **Test on a copy first** if you're paranoid (you should be)
- 🔴 **Don't run it on system folders** — pointing it at `C:/Windows/` is a great way to have a very bad afternoon
- 🟡 **Existing files are skipped** — NEUTRON will never silently overwrite anything
- 🟡 **`Misc/` is your safety net** — unrecognized files land there, not in the void
- 🟢 **Subfolders are preserved** — only regular files are moved; nested directories are left alone

If something goes wrong: your files are just in a different folder. Nothing is deleted. Breathe.

---

## 🧠 How This Was Built (The Honest Version)

I built NEUTRON while learning C++. Here's what actually happened:

**The AI part:** I was learning C++ from scratch and used an AI assistant (Claude, specifically) to get a working skeleton — the basic structure, the file iteration loop, the config parser pattern. Think of it like rubber-duck debugging but the duck can type.

**The me part:**
- Wrote and expanded the entire `neutron.conf` — all 500+ file type mappings, organized by category, from memory and research
- Added the "funny" comments throughout the code (the `// Skip spaces like you skip gym` energy is 100% mine)
- Extended the config parser to handle edge cases I hit during testing
- Debugged actual filesystem errors on Windows vs Linux
- Decided what the UX should feel like (the summary output, the `[OK]`/`[SKIP]`/`[ERR]` format)
- Ran it on my actual Downloads folder and survived

**The honest summary:** AI helped me learn and scaffolded the architecture. I built the actual thing on top of it, broke it repeatedly, fixed it, and made it mine. That's a valid way to learn. If you disagree, you probably also never used Stack Overflow.

---

## 📚 What I Learned

Building this taught me more than any tutorial:

- **`std::filesystem`** — the C++17 addition that makes file operations not terrible. `recursive_directory_iterator` is genuinely elegant.
- **RAII** — file streams close themselves when they go out of scope. Once you get it, you can't unsee it.
- **`std::map`** — O(log n) lookups, perfect for extension-to-folder mapping. No hash collisions, predictable iteration order.
- **CMake** — still confusing, but less so than before. The `target_compile_features(neutron PRIVATE cxx_std_20)` line took me an embarrassingly long time to get right.
- **Cross-platform pain** — `C:/Users/` vs `/home/user/` is just the beginning. Path separators, permissions, and "file in use" errors on Windows taught me patience.
- **Error handling** — `std::filesystem_error` is your friend. Mercury in retrograde is not a valid catch clause, but it felt appropriate to mention.

---

## 📄 License

MIT — do whatever you want with it. If you make it better, that'd be cool to see.

```
MIT License

Copyright (c) 2024 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

*NEUTRON out.*
