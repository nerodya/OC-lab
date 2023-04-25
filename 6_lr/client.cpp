#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

const int MSG_LEN = 1024;

const char* IP = "127.0.0.1";
const int PORT = 8081;

//args[1] - имя в чате
int main(int argv, char** args)
{
	//проверка аргументов
	if (argv != 3 || (args[2][0] != 's' && args[2][0] != 'r'))
	{
		printf("Incorrect input.\n");
		return 1;
	}
	
	int client;		//сокет клиента
	
	char client_request[256];
	char server_response[256];
	
	//адрес сервера
	struct sockaddr_in server_addr;
	
	//создание сокета клиента
	client = socket(AF_INET, SOCK_STREAM, 0);
	
	//формирование адреса для подключения
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port = htons(PORT);
	
	if(connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("Connection error\n");
		return 2;
	}
	else
	{
		//формируем привет серверу (имя + режим клиента)
		strcpy(client_request, args[1]);
		strcat(client_request, " ");
		strcat(client_request, args[2]);
		
		if(args[2][0] == 's')
		{
			send(client, client_request, sizeof(client_request), 0);
			
			while(1)
			{
				scanf("%s", client_request);
				send(client, client_request, sizeof(client_request), 0);
			}//..while
		}
		else
		if(args[2][0] == 'r')
		{
			send(client, client_request, sizeof(client_request), 0);
			while(1)
			{
				strcpy(client_request, "refresh");
				send(client, client_request, sizeof(client_request), 0);
				
				system("clear");
				
				recv(client, server_response, sizeof(server_response), 0);
				printf("%s\n", server_response);
				
				sleep(1);
			}
		}
		close(client);
	}
	return 0;
}