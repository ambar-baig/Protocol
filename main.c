#include "init.h"


int main(void){
    const char* message = "I AM PAKISTANI";
    const char* algorithm = CONST_MD5;
    unsigned char digest[64];
    fprintf(stdout,"Algorithm: %s\n", algorithm);
    Message_Digest(message, strlen(message), digest, algorithm);
    fprintf(stdout,"Message: %s\n", message);
    fprintf(stdout,"Digest: %ld\n", strlen(digest));

    return 0;
}
