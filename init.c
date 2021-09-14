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
            break;
        default:
            fprintf(stderr,"Error: Invalid Command.\n");
    }
}
void device_activator_wizard(void){
    char block[80];
    Digest_t* msg_digest = malloc(sizeof(Digest_t));
    //ignore_stream(); /* discard input buffer */
    fprintf(stdout,"Enter Qualified MAC Address: ");
    fscanf(stdin,"%s", msg_digest->device_mac);
    //msg_digest->device_mac[18] 
    digest_helper(msg_digest); /* SHA512 AND MD5 Helper function */

    /* DEBUG MODE */
    fprintf(stdout,"Device: %s\n", msg_digest->device_mac);
    fprintf(stdout,"Device SHA512: \n");
        print_digest(msg_digest->device_mac_sha512, 64);
    fprintf(stdout,"Device MD5: ");
        print_digest(msg_digest->device_mac_md5, 16);
    fprintf(stdout,"Device ID: ");
        print_digest(msg_digest->device_id, 16);
    free(msg_digest);

}
//28:ee:52:04:e3:c5
void device_authenticator_wizard(void){
    char input_digest[32];
    uint8_t device_id[16];
    Digest_t* msg_digest = malloc(sizeof(Digest_t));
    //ignore_stream(); /* discard input buffer */
    fprintf(stdout,"Enter Qualified MAC Address: ");
        fscanf(stdin,"%s", msg_digest->device_mac);
    fprintf(stdout,"Enter Device ID: ");
        fscanf(stdin,"%s", input_digest);
    /* compute digest */
    digest_helper(msg_digest);
    /* string to hexify */
    map(input_digest,device_id);
    /* compare device_ids*/
    if(memcmp(device_id, msg_digest->device_id, 16)== 0){
        fprintf(stdout,"Device Authenticated.\n");
    }else{
        fprintf(stdout,"Device Authentication Failed.\n");
    }
}
void digest_helper(Digest_t* msg){
    char block[80];
    
    Message_Digest(msg->device_mac, 8, msg->device_mac_md5, CONST_MD5);
    Message_Digest(&msg->device_mac[8], 17, msg->device_mac_sha512, CONST_SHA512);
    
    memcpy(block, msg->device_mac_sha512, 64);
    memcpy(&block[64], msg->device_mac_md5, 16);
    
    MD5(block, 80, msg->device_id); /* compute device id*/
    
}
void map(const char* string, uint8_t* output){
    char bytes[2];
    for(int i=0, j=0; i < 32; i += 2, j++){
        bytes[0] = string[i];
        bytes[1] = string[i+1];
        output[j] = strtol(bytes,'\0', 16);
    }
}

void ignore_stream(void){
    while ( getchar() != '\n' );
}
void print_digest(const uint8_t* digest, size_t count){
    for(int i = 0; i < count; i++){
        fprintf(stdout, "%x", digest[i]);
    }
    fprintf(stdout,"\n");
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
