#include "common.h"

#define false 0
#define true 1

char *cm_GetFileExtName(const char *file_name) {
    int f_index, l_index;
    char *f_ext = (char*) calloc(1, sizeof(char));
    char *l_ext = (char*) calloc(1, sizeof(char));
    size_t ext_size = 0;
    int is_ext_found = false;

    // Iterate backwards to find nearest dot for the extension
    for(f_index = (int) strlen(file_name) - 1; f_index >= 0; --f_index) {
        if(f_index && file_name[f_index] == 0x2E) {
            is_ext_found = true;
            break;
        }

        ext_size++;
        f_ext = (char*) realloc(f_ext, ext_size * sizeof(char));
        f_ext[ext_size - 1] = file_name[f_index];
    }

    // Exit function if no extension is found
    if(!is_ext_found) {
        free(f_ext);
        free(l_ext);
        return NULL;
    }

    // Rearrange extension chars in reverse order
    for(f_index = (int) strlen(f_ext) - 1, l_index = 0; l_index >= 0 && l_index < strlen(f_ext); f_index--, l_index++)
        l_ext[l_index] = f_ext[f_index];

    free(f_ext);
    return l_ext;
}


char *cm_TrimString(const char *str, size_t beg_index, size_t end_pos) {
    size_t index;
    char *end_str = calloc(end_pos - beg_index, sizeof(char));
    for(index = 0; beg_index < end_pos; beg_index++, index++)
        end_str[index] = str[beg_index];

    return end_str;
}


void cm_CheckMemoryAlloc(void *ptr) {
    if(ptr == NULL) {
        printf("ERROR: Failed to allocate memory!\n");
        exit(-1);
    }
}


void cm_CleanBuffer(char *ptr, size_t size) {
    for(size_t index = 0; index < size; index++)
        ptr[index] = 0x00;
}


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


void cm_LogWrite(const char *file_name, const char *content, int rewrite) {
    FILE *file;

    if(rewrite) file = fopen(file_name, "w");
    else file = fopen(file_name, "a");
    
    if(!file) {
        printf("ERRME: Failed to open log file for writing!\n");
        return;
    }

    fwrite(content, sizeof(const char), strlen(content), file);
    fwrite("\n", sizeof(char), 1, file);
    fclose(file);
}