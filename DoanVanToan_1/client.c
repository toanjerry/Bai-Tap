#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(5000);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");
    bzero(buffer,256);
    while(1){
	printf("Nhap thong bao: ");
	fgets(buffer,255,stdin);
	if(strlen(buffer) == 1 && buffer[0] == "q"){
	    close(sockfd);
	    return 0;
	}
	else{
	    n = write(sockfd,buffer,strlen(buffer));
    	    if (n < 0) error("ERROR writing to socket");
    	    n = read(sockfd,buffer,255);
    	    if (n < 0) error("ERROR reading from socket");
    	    printf("%s\n",buffer);	
	}
    }
    
}
