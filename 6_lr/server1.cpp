#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
// #include <vector>
#include <list>

#define PORT 8877
#define MAX_CLIENTS 10

const char *IP = "127.0.0.1";

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::list<int> clients;

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Присвоение сокету опции SO_REUSEADDR
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Задание параметров сервера
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(PORT);

    // Привязка сокета к адресу и порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание сокета
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиентов
    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "New client connected: " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

        // Добавление нового клиента в список
        clients.push_back(new_socket);
        valread = recv(new_socket, buffer, 1024, 0);
        // Обработка сообщений от клиента
        while (true)
        {
            // std::cout << "eeeeeeeeeeeeeeeeeeee";
            valread = recv(new_socket, buffer, 1024, 0);
            // std::cout << "rrrrrrrrrrrrrrrrrrrrrr";
            if (valread <= 0)
            {
                // Обработка отключения клиента
                std::cout << "Client disconnected: " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

                clients.remove(new_socket);
                close(new_socket);
                break;
            }
            // std::cout << "send " << buffer;
            // Отправка сообщения всем клиентам
            for (auto client : clients)
            {
                if (client != new_socket)
                {
                    send(client, buffer, sizeof(buffer), 0);
                }
            }

            // Очистка буфера
            memset(buffer, 0, sizeof(buffer));
        }
    }

    return 0;
}