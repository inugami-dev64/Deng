#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include <common/base_types.h>
#include <common/cerr_def.h>

char str[2048] = {0};
char sub_str[2048] = {0};

int main() {
    scanf("%[^\n]", str);
    char ch;
    scanf("%c", &ch);
    scanf("%[^s\n]", sub_str);
    
    deng_ui32_t ind_cap = 16;
    deng_ui32_t ind_c = 0;
    deng_ui32_t *indices = (deng_ui32_t*) calloc (
        ind_cap,
        sizeof(deng_ui32_t)
    );

    deng_ui32_t m, i;
    deng_ui32_t rep = UINT32_MAX;

    // Find all common repeating substring sequences
    for(m = 0, i = 0; m < strlen(str); i++, m++) {
        // Add substring location to indices and redo the loop
        if(i >= strlen(sub_str) - 1) {
            ind_c++;
            // Check if memory reallocation is needed
            if(ind_c >= ind_cap) {
                ind_cap <<= 1;
                deng_ui32_t *tmp = (deng_ui32_t*) calloc (
                    ind_cap,
                    sizeof(deng_ui32_t)
                );

                if(!tmp) MEM_ERR("indices");

                indices = tmp;
            }

            indices[ind_c - 1] = m - i; 
            rep = UINT32_MAX;
            i = 0;
        }

        // Check if current main string character matches substring first character
        if(str[m] == sub_str[0])
            rep = m;

        // Check if characters do not match and if they don't then set i to 0
        if(sub_str[i] != str[m]) {
            // No repeated characters found
            if(rep == UINT32_MAX) {
                m += i;
                i = 0;
            }
            // Found repeated character 
            else {
                i = 0;
                m = rep;
                rep = UINT32_MAX;
            }
        }
    }

    // Print all substring indices
    for(i = 0; i < ind_c; i++)
        printf("%d\n", indices[i]);

    free(indices);
    return EXIT_SUCCESS;
}
