.PHONY: all clean build iso qemu help configure clean-all

# Build directory
BUILD_DIR := build
SOURCE_DIR := .

# Default target
all: configure build iso

# Configure build system
configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build kernel and bootloader
build: configure
	@cd $(BUILD_DIR) && make -j$$(nproc)

# Create ISO image
iso: build
	@cd $(BUILD_DIR) && make iso

# Run in QEMU
qemu: iso
	@cd $(BUILD_DIR) && make qemu

# Clean build
clean:
	@cd $(BUILD_DIR) && make clean 2>/dev/null || true

# Clean everything
clean-all:
	@rm -rf $(BUILD_DIR)
	@echo "Clean build directory removed"

# Rebuild
rebuild: clean-all all

# Help
help:
	@echo "Hybrid OS Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make configure  - Configure build system with CMake"
	@echo "  make build      - Build kernel and bootloader"
	@echo "  make iso        - Create bootable ISO image"
	@echo "  make qemu       - Run OS in QEMU"
	@echo "  make clean      - Clean build artifacts"
	@echo "  make clean-all  - Remove entire build directory"
	@echo "  make rebuild    - Clean and rebuild everything"
	@echo "  make all        - Configure, build, and create ISO (default)"
	@echo ""
