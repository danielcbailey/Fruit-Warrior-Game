

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