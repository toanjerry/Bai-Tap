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
#include <pthread.h>
#include <fcntl.h>

#define PORT 5000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int numOfFile = 0;

void * doing(void *arg){
	int newsockfd = *((int *)arg);
	int rd, wrt;
	int numOfFileTemp = 0;
	//
	FILE * pFile;
    char sendbuff[256];
    char recvbuff[256];
	long lSize;
    size_t result;
    int  blockSize;

	free(arg);
	pthread_detach(pthread_self());

	while(1){
		//doc ten file
		bzero(recvbuff, 256);
		rd = read(newsockfd,recvbuff,255);

		if (rd < 0) printf("khong the read data\n");
		if(strlen(recvbuff) == 0 ||(strlen(recvbuff) == 4 && recvbuff[0] == 'Q' && recvbuff[1] == 'U' && recvbuff[2] == 'I' && recvbuff[3] == 'T')){
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
		pthread_mutex_lock(&lock);
		numOfFile++;
		pthread_mutex_unlock(&lock);		
		fclose (pFile);
	}
	
	printf("Exit socketThread and disconnect to client\n");
	close(newsockfd);
	printf("Number of file is dowloaded to the present: %d\n",numOfFile);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockfd;
	int *newsockfd;
    int clilen;
	struct sockaddr_in serv_addr, client_addr;

	pthread_t threadID[30];
	int i = 0;
   
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

    clilen = sizeof(client_addr);
    printf("server is listening at port %d\n",PORT);
    while(1){
		//accepting
		newsockfd = malloc(sizeof(int));
	    *newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clilen);
		if (newsockfd < 0){
			printf("Khong the accept\n");
			continue;
			
		}
		printf("\nclient IP: %s\n",inet_ntoa(client_addr.sin_addr));
		printf("client port: %d\n",client_addr.sin_port);

		if( pthread_create(&threadID[i], NULL, doing, newsockfd) != 0 )
		printf("khong the tao thread\n");
		if( i >= 30 ){
        	i = 0;
        	while(i < 30){
        		pthread_join(threadID[i++],NULL);
        	}
        	i = 0;
        }
		
    }
    //close(sockfd);
}