#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#define PORT 5000

int main(int argc, char *argv[])
{
     int sockfd; 
     int newsockfd;
     
     char sendbuff[256];
     char recvbuff[256];
     
     struct sockaddr_in serv_addr, client_addr;
   
     int readData,writeData, cli_len;
     
     //Tao socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) error("Khong the tao socket");
     
             
     //Khoi tao gia tri cho cac vung nho
     //memset(recvbuff, 0, 256);
     bzero(recvbuff,256);
     bzero(recvbuff,256);
     //memset(sendbuff, '0', 256);
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //memset(&serv_addr, '0', sizeof(serv_addr));
     //memset(&client_addr, '0', sizeof(client_addr));
     bzero((char *) &client_addr, sizeof(client_addr));
     
     //Thiet lap dia chi server
     serv_addr.sin_family = AF_INET;      //default
     serv_addr.sin_addr.s_addr = INADDR_ANY;  //ip server
     serv_addr.sin_port = htons(PORT);      //port number
     
     //bind socket
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
         error("khong the binding");
     
     //listening
     listen(sockfd,5);
     cli_len = sizeof(client_addr);

     while(1){
     	printf("Server is listening at port %d\n",PORT);
      	newsockfd = accept(sockfd, (struct sockaddr *) &client_addr,&cli_len);
      	if (newsockfd < 0) 
      	{
             error("Khong the accept");
             continue;
      	}
      	printf("Client IP: %s\n",inet_ntoa(client_addr.sin_addr));
      	printf("Client Port: %d\n",client_addr.sin_port);
        while(1){
	    //read data from socket 
      	    readData = read(newsockfd,recvbuff,255);
	    recvbuff[readData] = '\0';
      	    if (readData < 0) error("Khong the doc data tu socket");
      	    printf("required File name: %s\n",recvbuff);
     	    //write data via socket
      	    strcpy(sendbuff, "Server has got message\n");
      	    writeData = write(newsockfd,sendbuff,strlen(sendbuff));
      	    if (writeData < 0) error("Khong the ghi data vao socket");
	}      	      
      	close(newsockfd);    //Dong ket noi cua client
      	sleep(1);
     }       
     close(sockfd);
     return 0; 
}

