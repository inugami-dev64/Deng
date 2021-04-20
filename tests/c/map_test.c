#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <common/hashmap.h>

int main() {
    int values[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};   
    char *keys[10] = {
        "karu", 
        "ilves", 
        "rebane", 
        "kass", 
        "koer", 
        "kaslane", 
        "koerlane",
        "metskits",
        "kajakas",
        "tuvi"
    };
    
    Hashmap hm;
    newHashmap(&hm, 10);

    // Push values
    for(int i = 0; i < 10; i++)
        pushToHashmap(&hm, keys[i], strlen(keys[i]), &values[i]);

    // Read values by hashing
    printf("Getting values by hashing: ");
    for(int i = 0; i < 10; i++) {
        int *x = (int*) findValue(&hm, keys[i], strlen(keys[i]));
        printf("%d ", *x);
    }
    printf("\n");

    popFromHashmap(&hm, "karu", 4);
    popFromHashmap(&hm, "kajakas", 7);
    
    // Get the list of values
    void **elems = getHashmapList(&hm);
    printf("Getting values by listing: ");
    for(int i = 0; i < hm.used_size; i++) {
        printf("%d ", *(int*) elems[i]);
    }

    printf("\n");
    return EXIT_SUCCESS;
}
