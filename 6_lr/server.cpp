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


int main()
{	
	int server;		//сокет сервера
	int client;		//сокет клиента
	
	char client_request[256];
	char server_response[256];	
		
	//адрес сервера
	struct sockaddr_in server_addr;
	//адрес клиента
	struct sockaddr_in client_addr;
	
	int client_len = sizeof(client_addr);
	
	//создание сокета сервера
	server = socket(AF_INET, SOCK_STREAM, 0);
	
	//формирование адреса сервера
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port = htons(PORT);
	
	//закрепление адреса за сервером
	if(bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
	{
		printf("Mistake 1");
	}
	
	//прослушивание подключений
	
	if(listen(server, 5) != 0)
	{
		printf("Mistake 2");
	}
	
	//массив пользователей
	unsigned char users[16];
	//число пользователей
	int users_ct = 0;
	while(1)
	{
		// PIZDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		client = accept(server, (struct sockaddr*)&client_addr, (socklen_t *)&client_len);
		
		users[users_ct++] = (char)client;
		
		//создание дочернего процесса для нового клиента
		if(fork() == 0)
		{
			char name[256];	//имя клиента
			char buf[256];
			char mode;		//режим клиента
            			
			recv(client, client_request, sizeof(client_request), 0);
			
			for(int i = 0; i < strlen(client_request)-2; i++)
				name[i] = client_request[i];
			name[strlen(client_request)-2] = '\0';
			
			mode = client_request[strlen(client_request)-1];
			
			int histor = open("/home/rodion/ocCode/6_lr/chat_list", O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
			printf("%s-%c : connected\n", name, mode);
			
			strcpy(buf, name);
			strcat(buf, "-");
			strcat(buf, &mode);
			strcat(buf, " : connected\n");
			
			write(histor, buf, strlen(buf));
			
			close(histor);
			
			if(mode == 's')
			{
				while(1)
				{
					int histor = open("/home/rodion/ocCode/6_lr/chat_list", O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);
					
					int a = recv(client, client_request, sizeof(client_request), 0);
					
					if(a <= 0)
					{
						printf("%s-%c : disconnected\n", name, mode);
						
						strcpy(buf, name);
						strcat(buf, "-");
						strcat(buf, " ");
						buf[strlen(buf)-1] = mode;
						strcat(buf, " : disconnected\n");
						
						write(histor, buf, strlen(buf));
						close(histor);
						
						break;
					}
					
					printf("%s : %s\n", name, client_request);
					strcpy(buf, name);
					strcat(buf, " : ");
					strcat(buf, client_request);
					strcat(buf, "\n");
					
					write(histor, buf, strlen(buf));
					
					close(histor);
					
				}//..while
				
			}
			else
			if(mode == 'r')
			{
				while(1)
				{
					
					int histor = open("/home/rodion/ocCode/6_lr/chat_list", O_RDONLY, S_IRUSR|S_IWUSR);
					
					int a = recv(client, client_request, sizeof(client_request), 0);
					
					if(a <= 0)
					{
						printf("%s-%c : disconnected\n", name, mode);
						
						strcpy(buf, name);
						strcat(buf, "-");
						strcat(buf, " ");
						buf[strlen(buf)-1] = mode;
						strcat(buf, " : disconnected\n");
						
						write(histor, buf, strlen(buf));
						close(histor);
						
						break;
					}
					
					strcpy(server_response, "");
					
					if(strcmp(client_request, "refresh") == 0)
					{
						read(histor, server_response, 256);
						send(client, server_response, sizeof(server_response), 0);
					}
					else
					{
						strcpy(server_response, "ERROR");
						send(client, server_response, sizeof(server_response), 0);
					}	
					
					close(histor);
					
				}//..while
				
			}//if..else
						
		}//..if
		
	}//..while
	close(server);
	return 0;
}