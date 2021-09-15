#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>/* IPPROT_TCP */
#include<sys/socket.h>
#include<stdlib.h> /* atoi() exit()*/
#include<errno.h> /* */
#include<string.h> /* strerror() */

struct message{
    char* ssid;
    char* pass;
    size_t ssid_size;
    size_t pass_size;
};;

void die_with_message(int);
void encode_packet(uint8_t*, uint8_t*,uint8_t*);

int main(int argc, char** argv){
    if(argc > 5 || argc < 5){
        fprintf(stdout,"Error: Invalid arguments.\n");
        fprintf(stdout,"<IP> <PORT> <SSID> <PASSWORD>\n");
        return -1;
    }
    int client_file_handler;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    /* Initialize */
    server_addr.sin_family = AF_INET;
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) == -1)
        die_with_message(1);
    server_addr.sin_port = htons(atoi(argv[2]));
    
    if((client_file_handler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        die_with_message(2);
    while(connect(client_file_handler, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1);
    
    uint8_t block[1024];

    encode_packet(argv[3], argv[4], block);
    
    send(client_file_handler,block,strlen(block),0);
    
    fprintf(stdout, "Sent...\n");
    close(client_file_handler);
    return 0;
}
void die_with_message(int err_seq){
    fprintf(stderr, "%d==>%s\n", err_seq,strerror(errno));
    exit(EXIT_FAILURE);  
}
void encode_packet(uint8_t* ssid, uint8_t* password,uint8_t* block){
    size_t ssid_size = strlen(ssid);
    size_t pass_size = strlen(password);
    memcpy(block, ssid, ssid_size);
    memcpy(block+ssid_size, ";", 1);
    memcpy(block+ssid_size+1, password, pass_size);
    block[ssid_size+pass_size+2] = '\0';
}

