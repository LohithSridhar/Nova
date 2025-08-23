#include <string.h>

char *strrchr(const char *str, int value) {
    int assumed_last_address = -1;
    for (int i = 0; str[i]; i++) {
        if (str[i] == value) assumed_last_address = i;
    }

    return assumed_last_address == -1 ? NULL : (char *) (str + assumed_last_address);
}

// strrchr.c - strrchr is a reversed version of strchr, as it gives you the LAST version of the character you're looking for.