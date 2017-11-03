#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <string.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SERVER_PORT 5050

typedef struct mathopt
{
	int oprate;
	float value1;
	float value2;
}mopt;

******************************************
//子进程 函数
******************************************
void process_child(int client_socket)
{
	char command[20];
	struct mathopt *mp = (struct mathopt *)command;	

	float result = 0;
	char sendbuf[1024];

	while(1)
	{
		recv(client_socket,command,20,0);
		
		//add		
		if(mp->oprate == 0)
		{
			result = mp->value1 + mp->value2;
		}

		//minus
		else if(mp->oprate == 1)
		{
			result = mp->value1 - mp->value2;
		}

		//multiply
		else if(mp->oprate == 2)
		{
			result = mp->value1 * mp->value2;
		}

		//divide
		else if(mp->oprate == 3)
		{
			result = mp->value1 / mp->value2;
		}

		//quit
		else if(mp->oprate == 4)
		{
			break;
		}

		sprintf(sendbuf,"%f",result);
		send(client_socket,sendbuf,sizeof(sendbuf),0);
		printf("v1: %f,v2: %f,r: %f\n",mp->value1,mp->value2,result);
		
	}//while
	
	close(client_socket);
}
 
*****************************************
//主函数
*****************************************
int main()
{
	int server_socket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

	int addr_len = sizeof(clientAddr);

	//create socket
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket < 0)
	{
		printf("socket error\n");
		return 0;
	}

	//set	
	int yes = 1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

	//bind
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_socket,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0)
	{
		printf("bind error\n");
		return -1;
	}

	//listen
	if(listen(server_socket,5) < 0)
	{
		printf("listen error\n");
		return -1;
	}

	printf("listen port: %d\n",SERVER_PORT);

	while(1)
	{
		//connect
		int client_socket;
		client_socket = accept(server_socket,(struct sockaddr *)&clientAddr,(socklen_t *)&addr_len);
		if(client_socket < 0)
		{
			printf("accept error\n");
			return -1;
		}

		pid_t child_pid;
		child_pid = fork();

		if(child_pid > 0)
		{
			close(client_socket);
			continue;
		}			
		else if(child_pid == 0)			
		{
			close(server_socket);
			printf("i am the child process,my process id is %d\n",getpid());
			process_child(client_socket);	
			break;
		}
		else
		{
			printf("error\n");
			exit(0);
		}
		

	}//while
	close(server_socket);
	return 0;
}
