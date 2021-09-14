#include"init.h"




uint8_t* Message_Digest(const uint8_t* data, 
                        size_t data_len,
                        uint8_t* digest,
                        const char* algo_name){
        
    if(memcmp(algo_name, CONST_SHA512, strlen(CONST_SHA512)) == 0)
        algo_executor = SHA512;
    else if(memcmp(algo_name, CONST_MD5, strlen(CONST_MD5)) == 0)
        algo_executor = MD5;
    else
        die_with_message("Error: Invalid Algorithm.");
    algo_executor(data, data_len, digest);
    
    return digest;
}


uint8_t* Message_Cipher(const uint8_t* data,
                        size_t data_len,
                        uint8_t* cipher,
                        const uint8_t* iv,
                        const uint8_t* key){
    return NULL;                        
}

/* auxiliary function */
void main_command_list(void){
    int option;
    fprintf(stdout,"-: Device Wizard :-\n");
        fprintf(stdout,"1-Activate Device :-\n");
            fprintf(stdout,"2-Authenticate Device :-\n");
                fprintf(stdout,"3-Exit\n");
    fprintf(stdout,"Choose Option: <1-3>: ");
    fscanf(stdin, "%d", &option);
    switch(option){
        case 1:
            device_activator_wizard();
            break;
        case 2:
            device_authenticator_wizard();
            break;
        case 3:
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr,"Error: Invalid Command.\n");
    }
}
void device_activator_wizard(void){

    char device_mac[17]; /* MAC Address*/
    char device_mac_sha512[64]; /* SHA512 DIGEST*/
    char device_mac_md5[16]; /* MD5 DIGEST */
    fprintf(stdout,"Enter Qualified MAC Address: ");
    fscanf(stdout,"%s", device_mac);
    Message_Digest(device_mac, 8, device_mac_md5, CONST_MD5);
    Message_Digest(&device_mac[8], 17, device_mac_sha512, CONST_SHA512);
    fprintf(stdout,"Device: %s\n", device_mac);
    fprintf(stdout,"Device SHA512: %s\n", device_mac_sha512);
    fprintf(stdout,"Device MD5: %s\n", device_mac_md5);

}
void device_authenticator_wizard(void){

}

char* to_lowercase(const char* string){
    size_t string_size = strlen(string)+1;
    char* dup_string = malloc(string_size);
    
    memcpy(dup_string, string, string_size);
    while(*dup_string){
        *dup_string = tolower(*dup_string);
        dup_string++;
    }
    return dup_string;
}

void die_with_message(const char* message){
    fprintf(stdout,"%s\n", message);
    exit(EXIT_FAILURE);
}
