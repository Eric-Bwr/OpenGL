#pragma once

#include <cstdint>
#include <cstdio>
#include <malloc.h>
#include <cstring>
#include <cctype>
#include <tchar.h>

enum _Binary_buffer_error {
    BBE_OK = 0,
    BBE_CANT_FIND_FILE,
    BBE_CANT_ALLOC_MEM
};
typedef enum _Binary_buffer_error BinaryBufferError;
typedef enum _Binary_buffer_error& BinaryBufferErrorReference;
typedef enum _Binary_buffer_error const BinaryBufferErrorConst;
typedef enum _Binary_buffer_error* BinaryBufferErrorPointer;
typedef enum _Binary_buffer_error* const BinaryBufferErrorConstPointer;

struct _Binary_buffer {
    uint8_t* _byte_array;
    uint64_t _bytes;
    uint64_t _bytes_off;
};
typedef struct _Binary_buffer BinaryBuffer;
typedef struct _Binary_buffer& BinaryBufferReference;
typedef struct _Binary_buffer const BinaryBufferConst;
typedef struct _Binary_buffer* BinaryBufferPointer;
typedef struct _Binary_buffer* const BinaryBufferConstPointer;

void binaryBufferDestroy(BinaryBufferPointer buffer);
uint64_t binaryBufferPop64(BinaryBufferPointer buffer);
const char* binaryBufferPopString(BinaryBufferPointer buffer, uint64_t string_size);

typedef const char* Path;

BinaryBufferError binaryBufferReadFromFile(BinaryBufferPointer buffer, Path file_path);