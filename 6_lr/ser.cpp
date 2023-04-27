#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <thread>

#define PORT 8789
const char *IP = "127.0.0.1";
std::vector<int> clients;


void add_client(int fd, sockaddr_in address, int addrlen)
{
    while (true)
    {
        int new_socket;
        if ((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            break;
            exit(EXIT_FAILURE);
        }
        clients.push_back(new_socket);
        std::cout << "New client connected." << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::thread receiveThread(add_client, server_fd, address, addrlen);
    while (true)
    {
        
        // 

        // Send list of clients to all clients
        std::string clients_list = "Clients: ";
        for (int i = 0; i < clients.size(); i++)
        {
            clients_list += std::to_string(clients[i]) + " ";
        }
        for (int i = 0; i < clients.size(); i++)
        {
            send(clients[i], clients_list.c_str(), clients_list.length(), 0);
        }

        // Handle messages from client
        while (true)
        {
         
            
            // valread = recv(new_socket, buffer, sizeof(buffer), 0);
            if (valread == 0)
            {
                // Client disconnected
                // clients.erase(std::remove(clients.begin(), clients.end(), new_socket), clients.end());
                std::cout << "Client disconnected." << std::endl;
                sleep(1);
                // Send updated list of clients to all clients
                std::string clients_list = "Clients: ";
                for (int i = 0; i < clients.size(); i++)
                {
                    clients_list += std::to_string(clients[i]) + " ";
                }
                for (int i = 0; i < clients.size(); i++)
                {
                    send(clients[i], clients_list.c_str(), clients_list.length(), 0);
                }

                break;
            }
            sleep(1);
            std::cout << "Received message: " << buffer << std::endl;

            // Send message to all clients except sender
            for (int i = 0; i < clients.size(); i++)
            {
                std::cout << i;
                if (clients[i] != new_socket)
                {
                    send(clients[i], buffer, strlen(buffer), 0);
                }
            }
            std::cout << "2";
            memset(buffer, 0, sizeof(buffer));
            std::cout << "3";
        }
    }

    return 0;
}