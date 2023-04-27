#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

mutex mtx;

const int PORT = 8785;
const int BUFFER_SIZE = 1024;
const char *IP = "127.0.0.1";

void get_messages(int sockfd)
{
    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1)
        {
            cerr << "\nне удалось получить сообщение с сервера\n\n";
        }
        else if (bytes_received == 0)
        {
            cerr << "\nсервер отключен\n";
            close(sockfd);
            break;
        }
        else
        {
            string message(buffer);
            cout << message;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "\nнеправильнный формат ввода\n";
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IP);
    server_address.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        cerr << "\nошибка соединения с сервером\n";
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cout << "успешное подключение к серверу\n"; 

    future<void> receive_future = async(launch::async, get_messages, ref(sockfd));

    string message;
    string part_message;
    string nickname(argv[1]);

    int bytes_sent = send(sockfd, nickname.c_str(), nickname.length(), 0);
    if (bytes_sent == -1)
    {
        cerr << "\nне удалось отправить привет серверу\n\n";
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    mtx.lock();
    while (true)
    {
        cin.clear();
        message.clear();
        message += nickname + " : ";
        getline(cin, part_message);
        message += part_message + "\n";

        int bytes_sent = send(sockfd, message.c_str(), message.length(), 0);
        if (bytes_sent == -1)
        {
            cerr << "\nне удалось отправить сообщение серверу\n\n";
            break;
        }
    }
    mtx.unlock();
    receive_future.wait();
    close(sockfd);

    return 0;
}