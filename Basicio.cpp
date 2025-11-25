/* Basicio.cpp */
#include "Basicio.h"

// Declare the external Assembly function defined in syscall.s
extern "C" void low_level_write(const char* buffer, unsigned long len);

// Manual implementation of strlen to avoid <cstring>
unsigned long my_strlen(const char* str) {
    unsigned long len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void print(char* message) {
    low_level_write(message, my_strlen(message));
}

void print(const char* message) {
    low_level_write(message, my_strlen(message));
}