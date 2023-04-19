#include <ctype.h>

void codecA_encode(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (isupper(message[i])) {
            message[i] = tolower(message[i]);
        } else if (islower(message[i])) {
            message[i] = toupper(message[i]);
        }
    }
}

void codecA_decode(char *message) {
    codecA_encode(message);
}
