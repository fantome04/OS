#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int main(){
    struct sockaddr_in server_address;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(errno);
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9888);

    int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if(connected == -1){
        perror("connect");
        exit(errno);
    }

    std::string mssg;
    do{
        std::getline(std::cin, mssg);

        int sent = send(client_socket, mssg.c_str(), mssg.size(), 0);
        if(sent == -1){
            exit(errno);
        }
    } while(mssg != "exit");

    if(close(client_socket) < 0){
        perror("close");
        exit(errno);
    }
    return 0;
}