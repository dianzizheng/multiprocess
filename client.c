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

void opt_count(struct mathopt *pMp)
{
	printf("please input the first:");
	scanf("%f",&(pMp->value1));

	printf("please input the second:");
	scanf("%f",&(pMp->value2));
}

int main()
{
	int client_socket;
	
	struct sockaddr_in clientAddr;
	struct sockaddr_in serverAddr;

	struct mathopt mp;
	char command[20];
	char recvbuf[1024];

	//create socket
	client_socket = socket(AF_INET,SOCK_STREAM,0);

	//initialization port&ip
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.111");
	
	//connect
	if(connect(client_socket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
	{
		printf("connect error\n");
		return 0;
	}
	while(1)
	{
		printf("Input your word:\n");
		scanf("%s",command);

		float result = 0;

		//add
		if(strcmp(command,"add") == 0)
		{
			opt_count(&mp);
			mp.oprate = 0;
		}
		
		//minus
		else if(strcmp(command,"minus") == 0)
		{
			opt_count(&mp);
			mp.oprate = 1;
		}

		//multiply
		else if(strcmp(command,"multiply") == 0)
		{
			opt_count(&mp);
			mp.oprate = 2;
		}

		//divide
		else if(strcmp(command,"divide") == 0)
		{
			mp.oprate = 3;
			while(1)
			{
				opt_count(&mp);
				if(mp.value2 == 0)
				{
					printf("error: can't identify!\n");
					continue;
				}
				else
				{		
					break;
				}
			}		
		}	

		//quit
		else if(strcmp(command,"quit") == 0)
		{
			mp.oprate = 4;
			break;
		}

		else	
		{
			printf("error!\n");
			break;
		}

		send(client_socket,(char*)&mp,sizeof(mp),0);

		sprintf(recvbuf,"%f",result);
		recv(client_socket,recvbuf,1024,0);
		printf("recv data: %s\n",recvbuf);
		
	}//while

	close(client_socket);
	return 0;
}
