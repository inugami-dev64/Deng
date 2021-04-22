#include <stdio.h>
#include <string.h>
#include <stdlib.h>

wchar_t* cm_ToWideString(char* str) {
    wchar_t* out = (wchar_t*) calloc(strlen(str) + 1, sizeof(wchar_t));
    for (size_t i = 0; i < strlen(str); i++)
        out[i] = (wchar_t)str[i];

    // Siin toimib printf ilusti
    printf("%ls\n", out);
    printf("%p\n", out);
    return out;
}

int main(void) {
	wchar_t *win_title = cm_ToWideString("Hello world");
	printf("%p\n", win_title);
}
