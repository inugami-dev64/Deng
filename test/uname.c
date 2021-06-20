#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    struct utsname uname_data = { 0 };
    errno = 0;

    if(uname(&uname_data) < 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    printf("System name: %s\n", uname_data.sysname);
    printf("Node name: %s\n", uname_data.nodename);
    printf("Release version: %s\n", uname_data.release);
    printf("Version name: %s\n", uname_data.version);
    printf("Machine name: %s\n", uname_data.machine);

    #ifdef _GNU_SOURCE
        printf("Domain name: %s\n", uname_data.domainname);
    #endif

    return EXIT_SUCCESS;
}
