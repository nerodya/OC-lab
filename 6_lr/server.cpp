#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <mutex>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

using namespace std;

mutex mtx;

const int PORT = 8785;
const char *IP = "127.0.0.1";
constexpr int MAX_CLIENTS = 10;
constexpr int BUFFER_SIZE = 1024;

void send_messages(int client_socket, vector<int> &clients, int index, int &socket_count, string message)
{
    cout << message;
    mtx.lock();
    for (int i = 0; i < socket_count; i++)
        if (clients[i] != client_socket && clients[i] != -1)
            if (send(clients[i], message.c_str(), message.size(), 0) == -1)
                cerr << "не удалось отправить сообщение клиенту " << i << endl;

    mtx.unlock();
}

void handle(int client_socket, vector<int> &clients, int index, int &socket_count, string name)
{
    char buffer[BUFFER_SIZE];
    string message;
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0)
        {
            cerr << "\nклиент " << name << " отключился" << endl;
            close(client_socket);
            clients[index] = -1;
            socket_count--;
            index = -1;
            break;
        }

        message.clear();
        message = string(buffer, bytes_received);
        send_messages(client_socket, clients, index, socket_count, message);
    }
    close(client_socket);
}

int main(int argc, char const *argv[])
{
    vector<future<void>> futures(MAX_CLIENTS);
    vector<int> clients(MAX_CLIENTS, -1);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("ошибка при создании сокета");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IP);
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("ошибка при биндинге адреса");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("ошибка при установке слушателя");
        exit(1);
    }

    int index = -1;
    int socket_count = 0;

    while (true)
    {
        int len_addr = sizeof(server_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t *)&len_addr);

        if (client_socket == -1)
        {
            cerr << "клиент не подключен" << endl;
            continue;
        }
        else
        {
            if (socket_count < MAX_CLIENTS)
            {
                socket_count++;
                index++;
            }
            else
            {
                cerr << "достигнуто максимальное число клиентов" << endl;
                close(client_socket);
                continue;
            }
        }

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        cout << "подключен клиент " << string(buffer) << " с номером = " << index << endl;

        clients[index] = client_socket;
        for (int j = 0; j < MAX_CLIENTS; j++)
        {
            cout << "\nклиент " << j << " с сокетом = " << clients[j];
        }
        cout << "\n\n";

        futures[index] = async(launch::async, handle, client_socket, ref(clients), cref(index), ref(socket_count), string(buffer));
    }
    close(server_socket);
    return 0;
}