/*Group Members [Umar Iftikhar(15-11013) -Waleed Ahmed(16-10876)]*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


int sds[10];
int sd_index = 0;
char buffer[100]={' '};
//char curr_cli_num [4];//current client's number


//threading function
void *clients(void * arg)
{
		int i=0;

		int new_sd=*(int*)arg;	//present thread's file_descriptor

		
		char welcome_message[20];

		char str [4];//contains the present client's number

		sprintf(str, "%d", sd_index);

		strcpy (welcome_message, "Welcome Client ");
		strcat (welcome_message, str); 
		strcat (welcome_message, "\n");
		
		//this will send the <welcome_message>//string// on the console of client
		send(new_sd,welcome_message,strlen(welcome_message),0);
		//send(sds[sd_index-1],welcome_message,strlen(welcome_message),0);
		int open_q,close_q;	//for debugging purpose 
		while(1){
			char msg[100]={' '};
			int msg_length=0;
	

			//the inputs on the terminals will be received in the recv function
			recv (new_sd,buffer,100,0);
//*****************\SHOW MESSAGE ROUTINE****************/
			if (strstr(buffer, "\\SHOW"))
			{
				int i;
				for (i = 0; i < sd_index; i++)
				{
					if (sds[i] == -1)
					{
						continue;
					}

					char str1 [4];

					sprintf(str1, "%d", i+1);
					char client_show_message[10];
					strcpy (client_show_message, "Client ");
					strcat (client_show_message, str1);
					strcat (client_show_message, "\n"); 
					send (new_sd, client_show_message, strlen (client_show_message), 0);
				}
			}	
//*****************\END ROUTINE****************/
			else if (strstr(buffer, "\\END"))
			{
				int myIndex = 0; int k = 0;
		
				for (; k < sd_index; k++)		
				{
					if (new_sd == sds[k])
					{
						sds[k] = -1;
						break;
					}	
				}


				//char curr_cli_num [4];

				sprintf(str, "%d", k+1);	//string to integer
				char client_end_message[20];
				strcpy (client_end_message, "Bye Bye Client ");
				strcat (client_end_message, str);
				strcat (client_end_message, "\n"); 
				//to show the bye bye message on the client end
				send (new_sd, client_end_message, strlen (client_end_message), 0);
				//to show the disconnection message on the server side
				printf("Client %s disconnected\n",str);
				break;
			}
//*****************\PVT MESSAGE ROUTINE****************/
			else if(strstr(buffer, "\\PVT") && strstr(buffer, "Client")){
				if(buffer[0]=='\\' && buffer[1]=='P' && buffer[2]=='V' && buffer[3]=='T'){
					//finding the index to the opening quote
					//int open_q;	
						for(open_q=0; open_q<strlen(buffer);open_q++){
							if(buffer[open_q]=='\"'){				
								break;
							}
						}
					//finding the index to the closing quote
					//int close_q;
						for(close_q=open_q+1; strlen(buffer); close_q++){
							if(buffer[close_q]=='\"'){				
								break;
							}
						}
					//retrieving the message
					int siz_msg=close_q-open_q;
					char *message=(char*)malloc(close_q-open_q);
		
					int i;
					int j=open_q+1;
					for(i=0;i<siz_msg-1;i++){
						message[i]=buffer[j];
						j++;
					}
					
					
					//finding the recipient Client's number
					//char *c=(char *)malloc(1);		
					//*c=buffer[strlen(buffer)-3];/*buffer[strlen(buffer)-3]*/
					////char c=buffer[strlen(buffer)-3];						
					//int recipient=atoi((const char*)c);//constant character causes problem
					//int recipient=c-48;					
					int recipient;	//for debugging
					
					int testt;
					for(testt=close_q;testt<strlen(buffer);testt++){
						if(buffer[testt]=='C'){
							if( (buffer[testt+1]=='l') && (buffer[testt+2]=='i')&&(buffer[testt+3]=='e')&&(buffer 				[testt+4]=='n')&&(buffer[testt+5]=='t'))
								{
								recipient=buffer[testt+6]-48;		
								}
					}
				}
					//free(c); frees the allocated memory
					////printf("Last character: %c\n",c);
					/*printf("Recipient=%d\n",recipient);************/
					//finding whether or not that client exists
					
					if(!(recipient>sd_index)){
						if(sds[recipient-1]!=-1 && sds[recipient-1]!=new_sd){					
							char *package=(char*)malloc(strlen(message)+20);		
							char c_no[2];//client no recipient							
							sprintf(c_no, "%d", recipient);
							strcpy(package, "Client ");
							strcat(package, str);
							strcat(package, " says \"");
							strcat(package, message);
							strcat(package, "\"");
							strcat(package, "\n");
				//this string will be displayed on the server screen				
			  		  printf("MSG from Client %s to Client %d\n",str,recipient);
					//		printf("BUFFER:%S\n",buffer);							
							send(sds[recipient-1],package,strlen(package),0);
						}else{
							if(sds[recipient-1]==new_sd){
								char *self="You cannot send a message to yourself\n";
								send(new_sd,self,strlen(self),0);
								
							}else{
								char *temp="1The specified client doesn\'t exist\n";
								send(new_sd,temp,strlen(temp),0);
							}
						}
					}else{
						char *temp1="2The specified client doesn\'t exist\n";
						send(new_sd,temp1,strlen(temp1),0);
					}

				}else{
					char *temp2="Please consider the following syntax for private messages:\n\\PVT [Your message in quotations]: [Client#] \n";
					send(new_sd,temp2,strlen(temp2),0);
				}
			}else{
					char *temp3="Please consider the following syntax for private messages:\n\\PVT [Your message in quotations]: [Client#] \n";
					send(new_sd,temp3,strlen(temp3),0);
				}

		}

}





int main(){

	
	int sd;	//socket descriptor for SERVER
	//socket
	
	struct sockaddr_in server_addr, client_addr;
	sd = socket (AF_INET,SOCK_STREAM,0);
	server_addr.sin_family= AF_INET;//IPv4
	server_addr.sin_port = htons(8085);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//bind
	bind(sd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));



	//listen
	listen(sd,10);	//backlog:maximum no of clients that can comm to a server
	int size = sizeof (struct sockaddr_in);




	//threading part//
	pthread_t thread[10];
	int counter=0;

	while(counter<10){


		//accept
		int new_sd = accept (sd, (struct sockaddr *) &client_addr,&size);

		sds[sd_index] = new_sd;
		sd_index++;

		//printf("Client%d with ", counter+1);

		//to print the client IP//The code before the next printf was copied off from https://www.stackoverflow.com
		printf("%d.%d.%d.%d",(int)(client_addr.sin_addr.s_addr&0xFF),
		  (int)((client_addr.sin_addr.s_addr&0xFF00)>>8),
		  (int)((client_addr.sin_addr.s_addr&0xFF0000)>>16),
		  (int)((client_addr.sin_addr.s_addr&0xFF000000)>>24));

		printf(" has just connected\n");



		//------threading---------//


		 pthread_attr_t attr;	//thread object
		 pthread_attr_init(&attr);//thread initialize
		 pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);//
		 
		int Rv=pthread_create(&thread[counter],&attr, &clients,(void *)&new_sd);
		 //pthread_cancel(thread[counter]); 
		//pthread_join(thread[counter], NULL);

		++counter;

	}



return sd;
}
