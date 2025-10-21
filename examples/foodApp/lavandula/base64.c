#include "include/base64.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char base64Map[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

char *base64Encode(const char *const plain) {
    int counts = 0;
    char buffer[3];
    char *cipher = malloc(strlen(plain) * 4 / 3 + 4);
    
    if (!cipher) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    int i = 0, c = 0;

    for(i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if(counts == 3) {
            cipher[c++] = base64Map[buffer[0] >> 2];
            cipher[c++] = base64Map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base64Map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base64Map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if(counts > 0) {
        cipher[c++] = base64Map[buffer[0] >> 2];
        if(counts == 1) {
            cipher[c++] = base64Map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        } else {
            cipher[c++] = base64Map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base64Map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';
    return cipher;
}


char *base64Decode(const char *const cipher) {
    int counts = 0;
    char buffer[4];
    char *plain = malloc(strlen(cipher) * 3 / 4 + 1);
        
    if (!plain) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    int i = 0, p = 0;

    for(i = 0; cipher[i] != '\0'; i++) {
        int k;
        for(k = 0 ; k < 64 && base64Map[k] != cipher[i]; k++);
        buffer[counts++] = k;
        if(counts == 4) {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if(buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0';
    return plain;
}