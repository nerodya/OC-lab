#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <thread>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

const int MSG_LEN = 1024;
const int BUFFER_SIZE = 1024;

const char *IP = "127.0.0.1";
const int PORT = 8789;

void receiveMessage(int socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == -1) {
            std::cerr << "recv failed" << std::endl;
            break;
        }
        if (bytesReceived == 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }
}


int main(int argv, char **args)
{
	// if (argv != 3 || (args[2][0] != 's' && args[2][0] != 'r'))
	// {
	// 	printf("Incorrect input.\n");
	// 	return 1;
	// }

	int client;

	char client_request[256];
	char server_response[256];

	struct sockaddr_in server_addr;

	client = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port = htons(PORT);

	if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("Connection error\n");
		return 2;
	}
	else
	{
		std::thread receiveThread(receiveMessage, client);

		strcpy(client_request, args[1]);
		strcat(client_request, " ");
		strcat(client_request, args[2]);

		// if (args[2][0] == 's')
		// {
			send(client, client_request, sizeof(client_request), 0);

			while (1)
			{
				scanf("%s", client_request);
				send(client, client_request, sizeof(client_request), 0);



////////////////////////////////////////////////////////
				// strcpy(client_request, "refresh");
				// send(client, client_request, sizeof(client_request), 0);

				// // system("clear");

				// recv(client, server_response, sizeof(server_response), 0);
				// printf("%s\n", server_response);

				// sleep(1);

				/////////////////////////////////////////////
			}
		// }
		// else
		// {
		// 	if (args[2][0] == 'r')
		// 	{
		// 		send(client, client_request, sizeof(client_request), 0);
		// 		while (1)
		// 		{
		// 			strcpy(client_request, "refresh");
		// 			send(client, client_request, sizeof(client_request), 0);

		// 			system("clear");

		// 			recv(client, server_response, sizeof(server_response), 0);
		// 			printf("%s\n", server_response);

		// 			sleep(1);
		// 		}
		// 	}
		// }
		close(client);
	}
	return 0;
}