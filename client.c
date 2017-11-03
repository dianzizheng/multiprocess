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

int main()
{
	int client_socket;
	
	struct sockaddr_in clientAddr;
	struct sockaddr_in serverAddr;
	
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
		//此处为要实现功能语句
 
		send(client_socket,(char*)&mp,sizeof(mp),0);     //send
		
		sprintf(recvbuf,"%f",result);
		recv(client_socket,recvbuf,1024,0);           //recv
		printf("recv data: %s\n",recvbuf);
		
	}//while

	close(client_socket);
	return 0;
}
