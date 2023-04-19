#include <ctype.h>

void codecB_encode(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] = (char) (((int) message[i] + 3) % 256);
    }
}

void codecB_decode(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] = (char) (((int) message[i] - 3 + 256) % 256);
    }
}
