#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(){

int sd;

struct sockaddr_in serv_addr;
sd = socket (AF_INET,SOCK_STREAM,0);
serv_addr.sin_family=AF_INET;
serv_addr.sin_port =htons (8085);
serv_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
connect (sd,(struct sockaddr *) &serv_addr, sizeof (struct sockaddr));
	while(1){	
	char msg[100]={' '};
	char buffer[100]={' '};
	
	printf("Client: ");
	scanf("%79[^\n]%*c",buffer);
	int msg_length=strlen(buffer);	
	send(sd,buffer,msg_length,0);

	recv (sd,msg,100,0);
	printf("\nClient msg: %s \n",msg);
	
	}
return 0;
}
