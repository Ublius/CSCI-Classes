#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>


// INSECURE
int main() {
    unsigned char aesKey[16];
    srand(time(NULL));
    for (int i = 0; i < 16; i++) {
        aesKey[i] = rand() % 256;  
    }
    for (int i = 0; i < 16; i++) {
        printf("%02x", aesKey[i]);
    }
    printf("\n");
    return 0;
}


// SECURE
int main() {
    unsigned char aesKey[16];
    int urandom = open("/dev/urandom", O_RDONLY); 
    read(urandom, aesKey, 16);
    close(urandom);

    for (int i = 0; i < 16; i++) {
        printf("%02x", aesKey[i]);
    }
    printf("\n");
    return 0;
}