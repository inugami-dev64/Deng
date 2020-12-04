#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Get the file extension from file name
    char *cm_GetFileExtName(const char *file_name);
    // Trim string from beginning and end
    char *cm_TrimString(const char *str, size_t beg_index, size_t end_pos);

    // Memory allocation check
    void cm_CheckMemoryAlloc(void *ptr);
    // Set all the bytes to zero
    void cm_ClearBuffer(char *ptr, size_t size);

    // Sort all the strings in alphabetical order
    void cm_SortAlphabetically(char **buffer, size_t buffer_count);

#ifdef __cplusplus
}
#endif

#endif