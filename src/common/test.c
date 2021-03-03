#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <common/hashmap.h>

// numbers in pi
static unsigned int pi[32] = {
    3, 1, 4, 1, 5, 9, 2, 6,
    5, 3, 5, 8, 9, 7, 9, 3,
    2, 3, 8, 4, 6, 2, 6, 4,
    3, 3, 8, 3, 2, 7, 9, 5
};

int main() {
    Hashmap hm;
    newHashmap(&hm, 2);
    
    // Random keys
    char *keys[32] = {
        "stay", "limping", "gate", "lazy",
        "wish", "tree", "bouncy", "next",
        "poor", "nerve", "aftermath", "jam",
        "fact", "scarce", "uptight", "hideous",
        "unique", "sincere", "giant", "rough", 
        "free", "prefer", "annoy", "piquant",
        "sulky", "tent", "jazzy", "maniacal",
        "probable", "ludicrous", "hover", "join"
    };
   
    // Push elements into hash map
    for(int i = 0; i < 32; i++) {
        pushToHashmap (
            &hm, 
            keys[i], 
            strlen(keys[i]), 
            &pi[i]
        );
    }

    // Read from map
    for(int i = 0; i < 32; i++) {
        unsigned int *p_int = (unsigned int*) findValue (
            &hm, 
            keys[i],
            strlen(keys[i])
        );

        if(p_int)
            printf("%d", *p_int);
    }

    printf("\n");
    free(hm.map_data);
    return EXIT_SUCCESS;
}
