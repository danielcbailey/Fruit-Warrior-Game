

#include <cstdio>
#include "mbed.h"

#define DEBUG

extern UnbufferedSerial pc;

inline void printfdbg(const char* fmt, ...) {
    #ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    char buf[256];
    int length = std::vsnprintf(buf, 256, fmt, args);
    pc.write(buf, length + 1);
    va_end(args);
    #endif
}

//A safer version of malloc, free
//Originally intended to protect against heap corruption from an ISR, but
//due to the operation of new and delete this isn't very effective.
inline void* malloc_safe(size_t size) {
    __disable_irq();
    void* ret = malloc(size);
    __enable_irq();
    return ret;
}

inline void free_safe(void* ptr) {
    __disable_irq();
    free(ptr);
    __enable_irq();
}

//#define printMalloc(ptr) printfdbg("m: %p %s:%d\n", ptr, __FILE__, __LINE__)

//#define printFree(ptr) printfdbg("f: %p %s:%d\n", ptr, __FILE__, __LINE__)

#define printMalloc(ptr)
#define printFree(ptr)