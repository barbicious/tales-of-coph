#pragma once

#pragma region types

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef u32 b32;

#pragma endregion types


#pragma region string

#include <stddef.h>

typedef struct string {
    char *data;
    size_t length;
} string_t;

string_t string_new(const char *str);
void string_free(string_t *string);

#ifdef UTILS_IMPL

#include <stdlib.h>
#include <string.h>

string_t string_new(const char *str) {
    string_t ret;
    ret.data = strdup(str);
    ret.length = strlen(str);
    return ret;
}

void string_free(string_t *string) {
    if (string != nullptr) {
        free(string->data);
        string->data = nullptr;
        string->length = 0;
    }
}

#endif

#pragma endregion string