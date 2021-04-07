#include <stdlib.h>
#include <stdio.h>
#include <common/uuid.h>

int main() {
    char *uuid = uuid_Generate();
    printf("UUID: %s\n", uuid);
    return EXIT_SUCCESS;
}
