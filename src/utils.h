#pragma once

#pragma region types

#include <stdint.h>
#include <stdlib.h>

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


#pragma region random

#define RAND_RANG(min, max) (min + (rand() % (max - min + 1)))

#pragma endregion random


#pragma region string

#include <stddef.h>

typedef struct string {
    char* data;
    size_t length;
} string_t;

string_t string_create(const char* str);
void string_free(string_t* string);

#ifdef UTILS_IMPL

#include <stdlib.h>
#include <string.h>

string_t string_create(const char* str) {
    string_t ret;
    ret.data = strdup(str);
    ret.length = strlen(str);
    return ret;
}

void string_free(string_t* string) {
    if (string != nullptr) {
        free(string->data);
        string->data = nullptr;
        string->length = 0;
    }
}

#endif

#pragma endregion string


#pragma region list

/*
 * Probably should do this in a non
 * "cast a billion things from void* way" but idc
 * It works for now and performance will probably be fine
 */

typedef struct list {
    void** items;
    size_t length;
    size_t capacity;
} list_t;

list_t list_create();
void list_append(list_t* list, void* item);
void* list_get(list_t* list, size_t index);
void list_remove(list_t* list, size_t index);
void list_destroy(list_t* list);

#ifdef UTILS_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

list_t list_create() {
    list_t ret;
    ret.length = 0;
    ret.capacity = 2;
    ret.items = malloc(sizeof(*ret.items) * ret.capacity);
    return ret;
}

void list_append(list_t* list, void* item) {
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, list->capacity * sizeof(*list->items));
    }

    list->items[list->length++] = item;
}

void* list_get(list_t* list, size_t index) {
    if (index >= list->length) {
        printf("Attempted out of bounds access on list.");
        exit(0);
    }

    return list->items[index];
}

void list_remove(list_t* list, size_t index) {
    if (index >= list->length) {
        printf("Attempted out of bounds access on list.");
        exit(0);
    }

    memmove(list->items, list->items + index, (list->length - index) * sizeof(*list->items));
    list->length--;
}

void list_destroy(list_t* list) {
    // TODO: Log warning that i tried to free null list
    if (list != nullptr && list->items != nullptr) {
        free(list->items);
        list->items = nullptr;
        list->length = 0;
        list->capacity = 0;
    }
}

#endif

#pragma endregion list
