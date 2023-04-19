#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int compareFiles(FILE *file1, FILE *file2, int verbose, int ignoreCase) {
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int line = 1;
    int result = 0;
    while (1) {
        char *line1 = fgets(buffer1, BUFFER_SIZE, file1);
        char *line2 = fgets(buffer2, BUFFER_SIZE, file2);
        if (line1 == NULL && line2 == NULL) {
            break;
        }
        if (line1 == NULL || line2 == NULL) {
            result = 1;
            break;
        }
        if (ignoreCase) {
            if (strcasecmp(line1, line2) != 0) {
                result = 1;
                break;
            }
        } else {
            if (strcmp(line1, line2) != 0) {
                result = 1;
                break;
            }
        }
        line++;
    }
    if (verbose) {
        printf("%s\n", result == 0 ? "equal" : "distinct");
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 1;
    }
    FILE *file1 = fopen(argv[1], "r");
    if (file1 == NULL) {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    FILE *file2 = fopen(argv[2], "r");
    if (file2 == NULL) {
        printf("Cannot open file %s\n", argv[2]);
        fclose(file1);
        return 1;
    }
    int verbose = 0;
    int ignoreCase = 0;
    if (argc == 4) {
        if (strcmp(argv[3], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[3], "-i") == 0) {
            ignoreCase = 1;
        } else {
            printf("Invalid argument: %s\n", argv[3]);
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }
    int result = compareFiles(file1, file2, verbose, ignoreCase);
    fclose(file1);
    fclose(file2);
    return result;
}
