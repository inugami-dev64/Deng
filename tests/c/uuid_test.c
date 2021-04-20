#include <stdlib.h>
#include <stdio.h>
#include <common/uuid.h>

int main() {
    for(size_t i = 0; i < 20; i++) 
        printf("%s\n", uuid_Generate());

    return EXIT_SUCCESS;
}
