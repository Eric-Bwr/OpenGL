#include "GLFileStream.h"

void binary_buffer_destroy(Binary_buffer_pointer buffer) {
    free(buffer->_byte_array);
}

uint64_t binary_buffer_pop64(Binary_buffer_pointer buffer) {
    if(buffer->_bytes < buffer->_bytes_off + 8) return 0;
    return (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 0))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 8))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 16))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 24))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 32))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 40))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 48))
           | (buffer->_byte_array[buffer->_bytes_off++] * (1ull << 56));
}

const char* binary_buffer_pop_string(Binary_buffer_pointer buffer, uint64_t string_size) {
    char* _binary_string_buffer = (char*)malloc(string_size);
    memcpy(_binary_string_buffer, &buffer->_byte_array[buffer->_bytes_off], string_size);
    return _binary_string_buffer;
}

typedef const char* Path;

void binary_buffer_read_from_file(Binary_buffer_pointer buffer, Path file_path) {
    FILE* _f = fopen(file_path, "rb");
    if(_f == NULL) {
        buffer->_byte_array = NULL;
        return;
    }

    fseek(_f, 0, SEEK_END);
    uint64_t _len = ftell(_f);
    rewind(_f);

    buffer->_byte_array = (uint8_t*)malloc(_len);
    if(buffer->_byte_array == NULL) {
        return;
    }
    buffer->_bytes = _len;
    buffer->_bytes_off = 0;

    fread(buffer->_byte_array, 1, _len, _f);

    fclose(_f);
}