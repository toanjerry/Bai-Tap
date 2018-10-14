#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#define PORT 5000
void sig_chld(int signo);

int main(int argc, char *argv[])
{
    int sockfd, rd, wrt,newsockfd;
    int clilen;
    long lSize;
    size_t result;
    int  blockSize;

    pid_t				childpid;
    struct sockaddr_in serv_addr, client_addr;

    FILE * pFile;
    char sendbuff[256];
    char recvbuff[256];

   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) printf("Khong the tao socket\n");
  
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&client_addr, '0', sizeof(client_addr));
    //Thiet lap dia chi cua server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //binding
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    	printf("Khong the binding\n");
    	exit(5);
    } 

    listen(sockfd,5);
    //signal(SIGCHLD, sig_chld);

    clilen = sizeof(client_addr);
    printf("server is listening at port %d\n",PORT);
    while(1){
		//accepting
	    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clilen);
		if (newsockfd < 0){
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				printf("Khong the accept\n");
		}
		// tao tien trinh con
		childpid = fork();
		if (childpid == 0) {	/* child process */
			close(sockfd);	/* close listening socket */
   			printf("client IP: %s\n",inet_ntoa(client_addr.sin_addr));
			printf("client port: %d\n",client_addr.sin_port);
			while(1){
			    //doc ten file
			    memset(recvbuff, 0, 256);
			    rd = read(newsockfd,recvbuff,255);
			    if (rd < 0) printf("khong the read data\n");
			    if(strlen(recvbuff) == 5 && recvbuff[0] == 'Q' && recvbuff[1] == 'U' && recvbuff[2] == 'I' && recvbuff[3] == 'T'){
					break;
				}
			    printf("Required file name: %s\n",recvbuff);
			    //mo file
			    pFile = fopen(recvbuff,"rb");
			    if(pFile == NULL){
					printf("Khong the mo file\n");
			    	memset(sendbuff, 0, 256);
			    	strcpy(sendbuff,"1File khong ton tai hoac khong the mo file");
			    	wrt = write(newsockfd,sendbuff,strlen(sendbuff));
			    	if (wrt < 0) printf(" khong the ghi data vao socket\n");
			    	continue;
			    }
			    // obtain file size and send to client
		    	fseek (pFile , 0 , SEEK_END);
			    lSize = ftell (pFile);
			    printf("File size: %ld byte\n",lSize);
			    memset(sendbuff, 0, 256);
			    sprintf(sendbuff,"_File size : %ld byte",lSize);
			    wrt = write(newsockfd,sendbuff,strlen(sendbuff));
			    if (wrt < 0) printf(" khong the ghi data vao socket\n");
			    //doc file va gui den client
			    rewind (pFile);
			    int sizeToRead = lSize;
			    while(sizeToRead > 0){
			        if (sizeToRead >= 254){
				    	blockSize = 254;
		            }
		            else{
		                blockSize = sizeToRead;
		            }
			    	memset(sendbuff, 0, 256);
			    	sendbuff[0] = '2';
					if(blockSize < 254) sendbuff[0] = '3';
			        result = fread (&sendbuff[1], 1, blockSize, pFile);
			        sendbuff[result+1] = '\0';
				    wrt = write(newsockfd,sendbuff,strlen(sendbuff));
				    if (wrt < 0) printf(" khong the ghi data vao socket");
			        sizeToRead -= blockSize;
			   	}
			    fclose (pFile);
			}
			close(newsockfd);				/* process the request */
			exit(0);
		}
		else{
			close(newsockfd);
		}
		
    }
    close(sockfd);
}	
	
////////
void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
