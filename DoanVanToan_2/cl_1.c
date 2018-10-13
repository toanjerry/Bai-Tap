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

int main(int argc, char *argv[])
{
    int sockfd, readData, writeData,conn;
    struct sockaddr_in serv_addr;

    char sendbuff[256];
    char recvbuff[256];

    //Client nhan tham so hostname va port tu dong lenh
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(0);
    }
   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Khong the tao socket");
  
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendbuff, 0, 256);
    memset(recvbuff, 0, 256);

    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;        //Mac dinh
    serv_addr.sin_port = htons(PORT);
   
    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    //connect den server
    conn = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if( conn < 0)
    {
    printf("\n Error : Kong the connect\n");
	exit(1);
    }
    while(1){
	printf("file name: ");
	
    	fgets(sendbuff,255,stdin);
	//Gui ten file
	writeData = write(sockfd,sendbuff,strlen(sendbuff));
    	if (writeData < 0) error("Khong the ghi data vao socket");

	//Nhan du lieu tu server
    	readData = read(sockfd,recvbuff,255);
	if (readData < 0) error("Khong the doc data tu socket");
    	printf("Noi dung file tu server: %s\n",recvbuff);

    }
    close(sockfd); //Dong socket 
    return 0;
}
