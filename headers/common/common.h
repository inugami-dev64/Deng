#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
#define C_ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

#ifdef __COMMON_C
    #include <stdlib.h> // size_t, calloc()
    #include <string.h> // strlen(), 
    #include <stdio.h> // FILE
    #include <common/base_types.h>
    #define __DENG_LOG_INIT_MSG "#ENTRY POINT\n";
#endif


// Get the file extension from file name
char *cm_GetFileExtName(const char *file_name);

/************* String functions ****************/
// Trim string from beginning and end
char *cm_TrimString (
    const char *str, 
    size_t beg_index, 
    size_t end_pos
);
// Make all the chars into upper case letters
char *cm_MakeUpperCase(const char *str); 
// Make all chars into lower case
char *cm_MakeLowerCase(const char *str);
// Replace all whitespaces with underscores
char *cm_StrReplaceCh (
    const char *str,
    char find_ch,
    char rep_ch
);

// Memory allocation check
void cm_CheckMemoryAlloc(void *ptr);
// Set all the bytes to zero
void cm_ClearBuffer(char *str, deng_i32_t len);

// Sort all the strings in alphabetical order
void cm_SortAlphabetically(char **buffer, size_t buffer_count);
// Log writer
void cm_OpenLogger(char* file_name);
void cm_LogWrite(const char *content);
void cm_CloseLogger();


#ifdef __cplusplus
}
#endif

#endif
