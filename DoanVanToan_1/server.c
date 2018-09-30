#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
char* strupc(char* msg){
   int i = 0;
   int len = strlen(msg);
   for(i = 0 ; i < len; i++){
	msg[i] = toupper(msg[i]);
   }
   return msg;
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[1000],upcaseMsg[1000];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
  
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(5000);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     bzero(buffer,1000);
     while(1){
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) error("ERROR on accept");
	printf("client IP: %s\n",inet_ntoa(cli_addr.sin_addr));
	printf("client port: %d\n",cli_addr.sin_port);
	//bzero(buffer,1000);
	n = read(newsockfd,buffer,1000);
	if (n < 0) error("ERROR reading from socket");
	printf("Thong bao tu client: %s",buffer);
	strupc(buffer);
	n = write(newsockfd,buffer,strlen(buffer));
	if (n < 0) error("ERROR writing to socket");	
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
     


