#ifndef INIT_H
#define INIT_H

#include<openssl/md5.h> /* MD5() */
#include<openssl/sha.h> /* SHA256() SHA512()*/
#include<stdint.h> /* uint8_t uint16_t uint32_t uint64_t */
#include<ctype.h> /* toupper() tolower() */
#include<string.h> /* memcpy() memcmp() strlen() */
#include<stdlib.h> /* exit() EXIT_FAILURE EXIT_SUCCESS*/
#include<stdio.h> /* fprintf() printf() stdout,stdin, stderr */

#define CONST_SHA512 "sha512"
#define CONST_MD5    "md5"

typedef struct{
    char device_mac[18]; /* MAC Address*/
    char device_mac_sha512[64]; /* SHA512 DIGEST*/
    char device_mac_md5[16]; /* MD5 DIGEST */
    uint8_t device_id[16]; /* aes 256 bit */
}Digest_t;

/* parameters-> data, data_length, digest, algorithm name*/
uint8_t* Message_Digest(const uint8_t*, size_t, uint8_t*, const char*);
/* parameters-> data, data_length, cipher, iv, key*/
uint8_t* Message_Cipher(const uint8_t*, size_t, uint8_t*, const uint8_t*, const uint8_t*);
uint8_t* (*algo_executor)(const uint8_t*, size_t, uint8_t*);

/* auxiliary function */
void main_command_list(void);
void device_activator_wizard(void);
void device_authenticator_wizard(void);
void digest_helper(Digest_t*);
void map(const char*, uint8_t*);
void ignore_stream(void);
void print_digest(const uint8_t*, size_t);
char* to_lowercase(const char*);
void die_with_message(const char*);


#endif
