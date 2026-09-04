cmake_minimum_required(VERSION 3.20)
project(Hybrid-OS C ASM)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Detect host vs cross-compile
if(NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compilation not detected. Setting up x86_64-elf toolchain...")
endif()

# Architecture
set(ARCH x86_64)
set(PLATFORM pc)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler flags for kernel (bare-metal)
set(CMAKE_C_FLAGS "-O0 -g3 -Wall -Wextra -Werror -fno-pie -fno-stack-protector -march=x86-64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2")
set(CMAKE_ASM_FLAGS "-g")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -no-pie -T${CMAKE_SOURCE_DIR}/kernel/linker.ld")

# Enable ASM language
enable_language(ASM)

# Kernel source files
file(GLOB_RECURSE KERNEL_SOURCES 
    "kernel/src/**/*.c"
)

file(GLOB_RECURSE KERNEL_ASM_SOURCES
    "kernel/src/**/*.asm"
    "kernel/src/**/*.s"
)

# Boot source files
file(GLOB_RECURSE BOOT_SOURCES
    "boot/**/*.c"
)

file(GLOB_RECURSE BOOT_ASM_SOURCES
    "boot/**/*.asm"
    "boot/**/*.s"
)

# Create kernel executable
add_executable(hybrid-os.elf
    ${BOOT_SOURCES}
    ${BOOT_ASM_SOURCES}
    ${KERNEL_SOURCES}
    ${KERNEL_ASM_SOURCES}
)

# Include directories
target_include_directories(hybrid-os.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/kernel/include
    ${CMAKE_SOURCE_DIR}/boot/include
)

# Link with libgcc for bare-metal
target_link_libraries(hybrid-os.elf PRIVATE gcc)

# Custom target for ISO
add_custom_target(iso
    COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/make_iso.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hybrid-os.elf ${CMAKE_BINARY_DIR}/hybrid-os.iso
    DEPENDS hybrid-os.elf
    COMMENT "Building ISO image..."
)

# Custom target for QEMU
add_custom_target(qemu
    COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/run_qemu.sh ${CMAKE_BINARY_DIR}/hybrid-os.iso
    DEPENDS iso
    COMMENT "Running in QEMU..."
)

# Clean build
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Cleaning build directory..."
)

message(STATUS "Hybrid OS build configured")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Architecture: ${ARCH}")
