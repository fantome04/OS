#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

void client_handler(int client_socket){
    char buffer[3001];
    int rs;

    while((rs = recv(client_socket, buffer, 3000, 0)) > 0){
        std::cout << "Got message:\n";
        buffer[rs] = '\0';
        std::cout << buffer << "\n";
    }
    if (rs == -1) {
        perror("receive");
    }

    if(close(client_socket) < 0){
        perror("close");
        exit(errno);
    }
}

int main(){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        perror("socket");
        exit(errno);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9888);

    if(bind(server_socket, (struct sockaddr*)& server_address, sizeof(server_address)) < 0){
        perror("bind");
        exit(errno);
    }

    if(listen(server_socket, 1024) < 0){
        perror("listen");
        exit(errno);
    }

    std::cout << "waiting for connectoin\n";

    std::vector<std::thread> client_threads;

    while(true){
        int client_socket;
        struct sockaddr_in client_address;
        unsigned int client_addr_len = sizeof(client_address);

        if((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0){
            perror("accept");
            exit(errno);
        }

        std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

        client_threads.push_back(std::thread(client_handler, client_socket));
    }

    for (auto &thread : client_threads) {
        thread.join();
    }

    if(close(server_socket) < 0){
        perror("close");
        exit(errno);
    }
    return 0;
}