#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUFFER_SIZE 1024

void printUsage() {
    printf("Usage: copy <file1> <file2> [-v] [-f]\n");
}

void printVerbose(char *message, int verbose) {
    if (verbose) {
        printf("%s\n", message);
    }
}

int copyFile(char *srcPath, char *destPath, int verbose, int force) {
    int srcFd = open(srcPath, O_RDONLY);
    if (srcFd == -1) {
        printf("Cannot open %s\n", srcPath);
        return 1;
    }
    int destFd = open(destPath, O_WRONLY | O_CREAT | (force ? O_TRUNC : O_EXCL), 0644);
    if (destFd == -1) {
        if (force) {
            destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (destFd == -1) {
                printf("Cannot create %s\n", destPath);
                close(srcFd);
                return 1;
            } else if (verbose) {
                printf("Target file overwritten\n");
            }
        } else {
            printf("Target file exists\n");
            close(srcFd);
            return 1;
        }
    } else {
        printVerbose("Target file created", !verbose);
    }
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;
    while ((bytesRead = read(srcFd, buffer, MAX_BUFFER_SIZE)) > 0) {
        bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            printf("Error writing to %s\n", destPath);
            close(srcFd);
            close(destFd);
            return 1;
        }
    }
    if (bytesRead == -1) {
        printf("Error reading from %s\n", srcPath);
        close(srcFd);
        close(destFd);
        return 1;
    }
    close(srcFd);
    close(destFd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4 && argc != 5) {
        printUsage();
        return 1;
    }
    int verbose = 0;
    int force = 0;
    char *srcPath = NULL;
    char *destPath = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        } else {
            if (srcPath == NULL) {
                srcPath = argv[i];
            } else if (destPath == NULL) {
                destPath = argv[i];
            } else {
                printf("Too many arguments\n");
                printUsage();
                return 1;
            }
        }
    }
    if (srcPath == NULL || destPath == NULL) {
        printf("Too few arguments\n");
        printUsage();
        return 1;
    }
    int result = copyFile(srcPath, destPath, verbose, force);
    if (result == 0) {
        if (verbose) {
            printf("Success\n");
        }
    } else {
        printf("General failure\n");
    }
    return result;
}

