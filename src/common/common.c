#define __COMMON_C
#include <common/common.h>

static FILE* s_file;

/* Clear all the contents in char buffer */
void cm_ClearBuffer(char *str, deng_i32_t len) {
    deng_i32_t index;
    for(index = 0; index < len; index++) 
        str[index] = 0x00;
}


/* Get the file extension name */
char *cm_GetFileExtName(const char *file_name) {
    deng_i32_t l_index;
    char *ext;
    deng_i32_t ext_size = 0;
    int is_ext_found = false;
    
    // Find the extension index
    for(l_index = strlen(file_name) - 1; l_index >= 0; l_index--) {
        if(file_name[l_index] == 0x2E) {
            is_ext_found = true;
            break;
        }
    }

    if(!is_ext_found) return NULL;

    l_index++;
    // Allocate memory for file extension string
    ext_size = strlen(file_name) - l_index;
    ext = (char*) calloc (
        ext_size + 1,
        sizeof(char)
    );

    // Write extension into ext variable
    sprintf (
        ext,
        "%s",
        file_name + l_index
    );

    return ext;
}


/* Trim string to certain size */
char *cm_TrimString(const char *str, size_t beg_index, size_t end_pos) {
    size_t index;
    char *end_str = calloc(end_pos - beg_index, sizeof(char));
    for(index = 0; beg_index < end_pos; beg_index++, index++)
        end_str[index] = str[beg_index];

    return end_str;
}


/* Replace all chars with other char*/
char *cm_StrReplaceCh (
    const char *str,
    char find_ch,
    char rep_ch
) {
    char *out_str = (char*) str;
    for(size_t i = 0; i < strlen(out_str); i++) {
        if(out_str[i] == find_ch)
            out_str[i] = rep_ch;
    }

    return out_str;
}


/* Make all lowercase chars into uppercase chars */
char *cm_MakeUpperCase(const char *str) {
    char *out_str = (char*) str;
    
    for(size_t i = 0; i < strlen(out_str); i++) {
        if(out_str[i] > 0x60 && out_str[i] < 0x7B) 
            out_str[i] -= 0x20;
    }

    return out_str;
}


/* Make all uppercase chars into lowercase chars */
char *cm_MakeLowerCase(const char *str) {
    char *out_str = (char*) str;
    for(size_t i = 0; i < strlen(str); i++) {
        if(out_str[i] > 0x40 && out_str[i] < 0x5B)
            out_str[i] += 0x20;
    }

    return out_str;
}


/* Check for memory allocation error */
void cm_CheckMemoryAlloc(void *ptr) {
    if(ptr == NULL) {
        printf("ERROR: Failed to allocate memory!\n");
        exit(-1);
    }
}


/* Sort all buffer contents alphabetically */
void cm_SortAlphabetically(char **buffer, size_t buffer_count) {
    int is_sorted = false;
    size_t l_index, r_index;
    char *tmp1, *tmp2;
    
    for(l_index = 0; l_index < buffer_count; l_index++)
        printf("%s\n", buffer[l_index]);

    while(!is_sorted) {
        is_sorted = true;
        for(l_index = 0; l_index < buffer_count - 1; l_index++) {
            // Check every char to see if it is bigger then the char in next string 
            for(r_index = 0; r_index < strlen(buffer[l_index]) && r_index < strlen(buffer[l_index + 1]); r_index++) {
                if(buffer[l_index][r_index] > buffer[l_index + 1][r_index]) {
                    tmp1 = buffer[l_index];
                    tmp2 = buffer[l_index + 1];
                    buffer[l_index] = tmp2;
                    buffer[l_index + 1] = tmp1;
                    is_sorted = false;
                    break;
                }
    
                else if(buffer[l_index][r_index] < buffer[l_index + 1][r_index]) break;
            }
            
            // Change order if first string is longer than other one and both strings are equal in char count they have in common
            if((r_index == strlen(buffer[l_index]) || r_index == strlen(buffer[l_index + 1])) 
            && strlen(buffer[l_index]) > strlen(buffer[l_index + 1])) {
                tmp1 = buffer[l_index];
                tmp2 = buffer[l_index + 1];
                buffer[l_index] = tmp2;
                buffer[l_index + 1] = tmp1;
                is_sorted = false;
            }
        }
    } 
}


/*
 * Open new file for logging (write only)
 */
void cm_OpenLogger(char* file_name) {
    s_file = fopen(file_name, "wb");
    if (!s_file)
        fprintf(stderr, "Failed to open file '%s'\n", file_name);

    const char* msg = __DENG_LOG_INIT_MSG;
    fwrite (
        msg, 
        sizeof(char), 
        strlen(msg), 
        s_file
    );
}


/*
 * Close logger FILE instance 
 */
void cm_CloseLogger() {
    fclose(s_file);
    s_file = NULL;
}


/* 
 * Write content into log file 
 */
void cm_LogWrite(const char *content) {
    fwrite(content, sizeof(const char), strlen(content), s_file);
    fwrite("\n", sizeof(char), 1, s_file);
}
