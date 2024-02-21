#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

const std::string SERVER_IP = "217.113.2.233";
const int SERVER_PORT = 80;
const std::string FILENAME = "ysu.html";

int main(){
    std::string request = "GET / HTTP/1.1\r\n";
    request += "Host: www.ysu.am\r\n";
    request += "Connection: close\r\n\r\n";

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        perror("socket");
        exit(errno);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());

    if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
        perror("connect");
        if(close(client_socket) < 0){
            perror("close");
            exit(errno);
        }
        exit(errno);
    }

    if(send(client_socket, request.c_str(), request.length(), 0) == -1){
        perror("send");
        if(close(client_socket) < 0){
            perror("close");
            exit(errno);
        }
        exit(errno);
    }

    std::stringstream response_buffer;
    char buffer[4096];
    int bytes_received;
    while((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0){
        response_buffer.write(buffer, bytes_received);
    }

    if(bytes_received == -1){
        perror("receive");
        if(close(client_socket) < 0){
            perror("close");
            exit(errno);
        }
        exit(errno);
    }

    std::cout << response_buffer.str() << std::endl;

    std::ofstream outfile(FILENAME);
    if (!outfile) {
        std::cerr << "Error opening file for writing" << std::endl;
        if(close(client_socket) < 0){
            perror("close");
            exit(errno);
        }
        exit(errno);
    }

    outfile << response_buffer.str();
    outfile.close();

    if(close(client_socket) < 0){
        perror("close");
        exit(errno);
    }

    return 0;
}