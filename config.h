/*
 * Hybrid OS Configuration Header
 * Central configuration for kernel and subsystems
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Architecture */
#define ARCH_X86_64
#define PLATFORM_PC

/* Kernel Configuration */
#define KERNEL_VERSION "0.1.0"
#define KERNEL_NAME "Hybrid OS"

/* Memory Configuration */
#define PAGE_SIZE 4096
#define PAGE_SHIFT 12
#define KERNEL_VIRT_BASE 0xffffffff80000000
#define KERNEL_PHYS_BASE 0x00100000

/* SMP Configuration */
#define MAX_CPUS 256
#define BOOT_STACK_SIZE 8192

/* IPC Configuration */
#define MAX_IPC_CHANNELS 4096
#define MAX_MESSAGES_PER_CHANNEL 256

/* Process Configuration */
#define MAX_PROCESSES 32768
#define MAX_THREADS_PER_PROCESS 256

/* Scheduler Configuration */
#define SCHEDULER_TICKS_PER_SECOND 1000
#define DEFAULT_TIME_SLICE 100 // milliseconds

/* Filesystem Configuration */
#define MAX_OPEN_FILES 4096
#define MAX_MOUNT_POINTS 256
#define VFS_CACHE_SIZE (64 * 1024 * 1024) // 64 MB

/* Security Configuration */
#define ENABLE_KPTI 1
#define ENABLE_SMEP 1
#define ENABLE_SMAP 1
#define ENABLE_NX 1

/* Debug Configuration */
#define DEBUG_ENABLED 1
#define SERIAL_DEBUG 1
#define SERIAL_PORT 0x3F8
#define SERIAL_BAUDRATE 115200

/* Feature Flags */
#define FEATURE_AI 1
#define FEATURE_VIRTUALIZATION 1
#define FEATURE_CONTAINERS 1
#define FEATURE_WIN32_COMPAT 1
#define FEATURE_POSIX_COMPAT 1

#endif // __CONFIG_H__
