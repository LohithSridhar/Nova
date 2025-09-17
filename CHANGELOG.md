# Changelog

All notable changes to this project will be documented in this file.

## Nova Muon [v0.1.9.1] - 12-Sep-2025
### Added
- Paging without disk switching
### Improved
- Memory allocation logic
- Code organisation and compilation
### Fixed
- Error where the heap initialiser would just not work

---

## Nova Electron [v0.1.9.0] - 12-Sep-2025
### Added
- Stack Smash Protection
### Improved
- Breakpoint Interrupt Logic (at least it's readable)
- Interrupt Handling Logic
### Fixed
- Error where going into Neutrino would randomly just freeze the keyboard

---

## Nova Electron [v0.1.8.9] - 23-Aug-2025
### NOTE: THIS IS THE FIRST ADDED ENTRY
### Added
- Kernel text scrolling and Internal Kernel Debugger (IKD)
- Breakpoint interrupt support
- ANSI Escape Key support
- Bootsplash with dynamic naming (e.g., XP, Vista)
- Switchable drivers with an agnostic kernel design
- A proper README.md and LICENSE, along with this very CHANGELOG.md

### Improved
- Fully modular build system for easier configuration
- Heap memory management improvements (malloc, free, calloc, realloc)
- Screen loading/saving for smoother display handling

### Fixed
- Various minor bugs and performance issues

---