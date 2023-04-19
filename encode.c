#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "encode.h"

void printUsage() {
    printf("Usage: encode <codec> <message>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }
    void *handle;
    char *error;
    void (*encode)(char *);
    if (strcmp(argv[1], "codecA") == 0) {
        handle = dlopen("./libcodecA.so", RTLD_LAZY);
        if (handle == NULL) {
            printf("Cannot load codec library\n");
            return 1;
        }
        encode = dlsym(handle, "codecA_encode");
    } else if (strcmp(argv[1], "codecB") == 0) {
        handle = dlopen("./libcodecB.so", RTLD_LAZY);
        if (handle == NULL) {
            printf("Cannot load codec library\n");
            return 1;
        }
        encode = dlsym(handle, "codecB_encode");
    } else {
        printf("Invalid codec: %s\n", argv[1]);
        return 1;
    }
    char *message = argv[2];
    encode(message);
    printf("%s ", message);
    dlclose(handle);
    return 0;
}
