#pragma once

#include <cstdint>
#include <cstdio>
#include <malloc.h>
#include <cstring>
#include <cctype>
#include <tchar.h>

struct _Binary_buffer {
    uint8_t* _byte_array;
    uint64_t _bytes;
    uint64_t _bytes_off;
};

typedef _Binary_buffer Binary_buffer;
typedef _Binary_buffer& Binary_buffer_reference;
typedef _Binary_buffer const Binary_buffer_const;
typedef _Binary_buffer* Binary_buffer_pointer;
typedef _Binary_buffer* const Binary_buffer_const_pointer;

void binary_buffer_destroy(Binary_buffer_pointer buffer);
uint64_t binary_buffer_pop64(Binary_buffer_pointer buffer);
const char* binary_buffer_pop_string(Binary_buffer_pointer buffer, uint64_t string_size);

typedef const char* Path;

void binary_buffer_read_from_file(Binary_buffer_pointer buffer, Path file_path);