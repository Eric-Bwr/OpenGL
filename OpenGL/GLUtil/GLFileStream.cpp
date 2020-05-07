#include "GLFileStream.h"

void binaryBufferDestroy(BinaryBufferPointer buffer) {
    free(buffer->_byte_array);
}

uint64_t binaryBufferPop64(BinaryBufferPointer buffer) {
    if (buffer->_bytes < buffer->_bytes_off + 8) return 0;
    return (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 0))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 8))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 16))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 24))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 32))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 40))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 48))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 56));
}

const char *binaryBufferPopString(BinaryBufferPointer buffer, uint64_t stringSize) {
    char *BinaryStringBuffer = (char *) malloc(stringSize);
    memcpy(BinaryStringBuffer, &buffer->_byte_array[buffer->_bytes_off], stringSize);
    return BinaryStringBuffer;
}

typedef const char* Path;

_Binary_buffer_error binaryBufferReadFromFile(BinaryBufferPointer buffer, Path filePath) {
    FILE *F = fopen(filePath, "rb");
    if (F == NULL) {
        buffer->_byte_array = NULL;
        return _Binary_buffer_error::BBE_CANT_FIND_FILE;
    }

    fseek(F, 0, SEEK_END);
    uint64_t _len = ftell(F);
    rewind(F);

    buffer->_byte_array = (uint8_t *) malloc(_len);
    if (buffer->_byte_array == NULL) {
        return _Binary_buffer_error::BBE_CANT_ALLOC_MEM;
    }
    buffer->_bytes = _len;
    buffer->_bytes_off = 0;

    fread(buffer->_byte_array, 1, _len, F);

    fclose(F);
    return _Binary_buffer_error::BBE_OK;
}