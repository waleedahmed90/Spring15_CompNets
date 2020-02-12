#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>

int sds[5];

int sd_index = 0;


char buffer[100]={' '};

void *chat_func(void * arg)
{
int i=0;

printf("connected thread %d \n",*(int*)arg);
int new_sd=*(int*)arg;

while(1){
	char msg[100]={' '};
	int msg_length=0;
	
	
	recv (new_sd,buffer,100,0);
	printf("\nClient: %s ",buffer);

	//printf("\nserver: ");
	//scanf("%79[^\n]%*c",msg);
	msg_length=strlen(buffer);	

	int j;
	for (j = 0; j <= sd_index; j++)
	{
		send(sds[j],buffer,msg_length,0);
	}

}


}

int main(){

pthread_t thread[5];
int counter=0;



int sd;
struct sockaddr_in my_addr, client_addr;
sd = socket (AF_INET,SOCK_STREAM,0);
my_addr.sin_family= AF_INET;
my_addr.sin_port = htons(8085);
my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
bind(sd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));


listen(sd,5);
int size = sizeof (struct sockaddr_in);



while(counter<5){



int new_sd = accept (sd, (struct sockaddr *) &client_addr,
&size);

sds[sd_index] = new_sd;
sd_index++;

if (sd_index == 5)
{
sd_index = 4;
}
//------threading---------


 pthread_attr_t attr;
 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
 int Rv=pthread_create(&thread[counter],&attr, &chat_func,(void *)&new_sd);
 //pthread_join(thread[counter], NULL);

++counter;

}



return 0;
}
