
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); //can be treated as a file. 
    if(server_fd < 0){ // if error is there in opening a socket.
        perror("Socket failed"); 
        exit(EXIT_FAILURE);
    }
    //using bind to bind it to a port.
    struct sockaddr_in server_address; // it has three things AF_INET for Ipv4 and port number and ip address.
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    //Actually binding
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    //second arguement here defines the number of people or pending connections allowed.
    if (listen(server_fd, 3) < 0) { // second argument is the maximum number of pending connection requests allowed
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    //client 
    struct sockaddr_in client_addr;
    struct sockaddr_in client_addr2;
    socklen_t addr_len = sizeof(client_addr);
    socklen_t addr_len2 = sizeof(client_addr2);

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    int client_fd2 = accept(server_fd, (struct sockaddr*)&client_addr2, &addr_len2);    
    if(client_fd <0 ||  client_fd2<0){
        printf("Connection failed");
        return 0;
    }
    printf("Connection Established.\n");
    char punish[BUFFER_SIZE]="You have 1 year in jail.";
    char punish2[BUFFER_SIZE]="You are free";
    char punish3[BUFFER_SIZE]="You have 2 year in jail.";
    char punish4[BUFFER_SIZE]="You have 3 year in jail.";

    while(1){
        char buffer[BUFFER_SIZE] = {0};
        char buffer2[BUFFER_SIZE] = {0};
        int b1=read(client_fd, buffer, BUFFER_SIZE);
        int b2=read(client_fd2, buffer2, BUFFER_SIZE);
        if(b1==0 || b2==0 ){
            break;
        }
        if(buffer[0]=='1' && buffer2[0]=='1'){
            write(client_fd2,punish,BUFFER_SIZE);
            write(client_fd,punish,BUFFER_SIZE);
            printf("Each have 1 yr of jail\n");
        }

        if(buffer[0]=='0' && buffer2[0]=='1'){
            write(client_fd2,punish4,BUFFER_SIZE);
            write(client_fd,punish2,BUFFER_SIZE);
            printf("Prisoner 1 is free and Prisoner 2 has 3 yr of jail\n");
        }
        if(buffer[0]=='1' && buffer2[0]=='0'){
            write(client_fd2,punish2,BUFFER_SIZE);
            write(client_fd,punish4,BUFFER_SIZE);
            printf("Prisoner 1 has 3 yr of jail and Prisoner 2 is free\n");
        }
        if(buffer[0]=='0' && buffer2[0]=='0'){
            write(client_fd2,punish3,BUFFER_SIZE);
            write(client_fd,punish3,BUFFER_SIZE);
            printf("Both Prisoner have 3 yr of jail\n");
        }   
        fflush(stdout);
        
    }
    close(client_fd2);
    close(client_fd);
    close(server_fd);

    return 0;
        

}