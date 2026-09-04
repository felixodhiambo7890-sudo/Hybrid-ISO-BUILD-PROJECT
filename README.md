# Hybrid-ISO-BUILD-PROJECT

A comprehensive hybrid operating system supporting x86_64 architecture with native POSIX/Linux and Windows Win32 compatibility, advanced kernel features, AI integration, virtualization, containerization, and modern security.

## Project Status: PHASE 0 - BUILD SYSTEM

### Quick Start

```bash
# Initial setup
bash setup_boot.sh

# Configure build system
make configure

# Build kernel
make build

# Create bootable ISO
make iso

# Run in QEMU
make qemu
```

## Requirements

- **Cross-compiler**: `x86_64-elf-gcc`
- **Build system**: `cmake >= 3.20`, `make`
- **ISO tools**: `xorriso`
- **Emulator**: `qemu-system-x86_64`
- **Assembler**: `nasm`

### Ubuntu/Debian Installation

```bash
sudo apt update
sudo apt install build-essential cmake xorriso qemu-system-x86 nasm

# Install cross-compiler
sudo apt install gcc-10-multilib g++-10-multilib
# Or use: https://github.com/lordmilko/i686-elf-tools
```

### Fedora Installation

```bash
sudo dnf install gcc gcc-c++ cmake xorriso qemu-system-x86 nasm
```

## Build System

### CMake

Uses CMake for cross-platform build configuration:
- `CMakeLists.txt` - Main build configuration
- `Toolchain.cmake` - x86_64-elf cross-compiler setup

### Make Targets

```bash
make help          # Show all targets
make configure     # Configure with CMake
make build         # Build kernel and bootloader
make iso           # Create bootable ISO
make qemu          # Run in QEMU emulator
make clean         # Clean build artifacts
make clean-all     # Remove entire build directory
make rebuild       # Clean and rebuild
```

## Directory Structure

```
Hybrid-ISO-BUILD-PROJECT/
├── boot/               # Bootloader code
├── kernel/             # Kernel source and headers
├── scripts/            # Build and utility scripts
├── tools/              # Development tools
├── tests/              # Test suite
├── docs/               # Documentation
├── CMakeLists.txt      # CMake configuration
├── Makefile            # Make build targets
├── Toolchain.cmake     # Cross-compiler setup
└── config.h            # Kernel configuration
```

## Development Phases

### Phase 0: Build System ✅
- CMake/Make build system
- Toolchain setup (x86_64-elf)
- QEMU/ISO configuration
- Scripts for building and testing

### Phase 1: Boot + Kernel Entry
- Multiboot2/Limine bootloader integration
- Early kernel entry point
- Serial output
- Framebuffer setup

### Phase 2-25: [See full roadmap in project structure]

## Debugging

### QEMU with GDB

```bash
# Terminal 1: Start QEMU with debug stub
qemu-system-x86_64 -s -S -cdrom build/hybrid-os.iso

# Terminal 2: Connect GDB
gdb build/bin/hybrid-os.elf
(gdb) source scripts/gdb_kernel.init
```

### Serial Output

Serialoutput redirected to `logs/qemu.log` for debugging.

### Symbol Map

Generate symbol map for stack traces:

```bash
python3 scripts/symbol_map.py build/bin/hybrid-os.elf
```

## Contributing

This is an educational/research project. Each phase has specific gates and testing requirements.

## License

MIT License - See LICENSE file for details

## Roadmap

- Phase 0: Build System ✅
- Phase 1: Boot + Kernel Entry
- Phase 2: GDT, IDT, Exceptions
- Phase 3: Memory Management
- Phase 4: Timer + SMP
- Phase 5: Scheduler
- Phase 6: Syscalls + Usermode
- Phases 7-25: See documentation

## Status

**Current Phase**: 0 - Build System Setup  
**Last Updated**: 2026-09-04
