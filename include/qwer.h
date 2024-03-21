#ifndef QWER_H
#define QWER_H

#if defined(_WIN32)
#define OS "win32"
#elif defined(__linux__)
#define OS "linux"
#elif defined(__APPLE__)
#define OS "darwin"
#else
#define OS "unknown"
#endif

#if defined(__i386__)
#define CPU "ia32"
#elif defined(__x86_64__)
#define CPU "x64"
#elif defined(__arm__)
#define CPU "arm"
#elif defined(__aarch64__)
#define CPU "arm64"
#else
#define CPU "unknown"
#endif

#if defined(__GLIBC__)
#define LIBC "glibc"
#elif defined(__MUSL__)
#define LIBC "musl"
#else
#define LIBC "unknown"
#endif

#endif