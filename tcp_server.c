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
    char* password;
    size_t ssid_size;
    size_t pass_size;
};

void die_with_message(int);
void decode_packet(const uint8_t*, uint8_t*,uint8_t*);

int main(int argc, char** argv){
    if(argc > 3 || argc < 3){
        fprintf(stdout,"Error: Invalid arguments.\n");
        fprintf(stdout,"<IP> <PORT>\n");
        return -1;
    }
    int server_file_handler;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    /* Initialize */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if((server_file_handler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        die_with_message(1);
    if(bind(server_file_handler, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        die_with_message(2);
    if(listen(server_file_handler, SOMAXCONN) == -1)
        die_with_message(3);
    
    fprintf(stdout,"Server is listening on [IP=%s][PORT=%s]\n", argv[1],argv[2]);
    int count = 0;
    while(count <= 2){
        struct sockaddr client_addr;
        struct message msg;
        uint8_t block[1024];

        socklen_t addr_len = sizeof(struct sockaddr);
        int client_file_handler;
        if((client_file_handler=accept(server_file_handler, &client_addr, &addr_len)) == -1)
            die_with_message(4);
        ssize_t size = recv(client_file_handler, block, 1024,0);
        char ssid[512];
        char password[512];
        decode_packet(block, ssid,password);
        
        fprintf(stdout,"SSID: %s\n",  ssid);
        fprintf(stdout,"PASSWORD: %s\n",  password);
        close(client_file_handler);
        count++;
    }
    close(server_file_handler);
    return 0;
}
void die_with_message(int err_seq){
    fprintf(stderr, "%d==>%s\n", err_seq,strerror(errno));
    exit(EXIT_FAILURE);  
}
void decode_packet(const uint8_t* block, uint8_t* ssid, uint8_t* password){
    char* duplicate = strdup(block);
    char* delimiters = ";\n";
    char* parse_ssid = strtok(duplicate, delimiters);
    char* parse_pass = strtok(NULL, delimiters);
    memcpy(ssid,  parse_ssid, strlen(parse_ssid));
    ssid[strlen(parse_ssid)] = '\0';
    memcpy(password, parse_pass, strlen(parse_pass));
    password[strlen(parse_pass)] = '\0';
           
}

