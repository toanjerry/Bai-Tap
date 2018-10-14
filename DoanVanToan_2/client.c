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
#include <stdio_ext.h>

#define PORT 5000

int main(int argc, char *argv[])
{
    int sockfd, rd, wrt,conn;
    struct sockaddr_in serv_addr;

    FILE * pFile;
    char sendbuff[256];
    char recvbuff[256];

    //Client nhan tham so hostname va port tu dong lenh
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(0);
    }
   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) printf("Khong the tao socket\n");
  
    memset(&serv_addr, '0', sizeof(serv_addr));
    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
   
    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("inet_pton error occured\n");
        return 1;
    }
    //connect den server
    conn = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if( conn < 0)
    {
	    printf("\n Error : Khong the connect\n");
		exit(1);
    }
    while(1){
		memset(sendbuff, 0, 256);
		__fpurge(stdin);
		printf("file name (10kb and 5mb): ");	
    	fgets(sendbuff,255,stdin);
	
		//Gui ten file
		wrt = write(sockfd,sendbuff,strlen(sendbuff)-1);
    	if (wrt< 0) printf("Khong the ghi data vao socket\n");
		if(strlen(sendbuff) == 5 && sendbuff[0] == 'Q' && sendbuff[1] == 'U' && sendbuff[2] == 'I' && sendbuff[3] == 'T'){
			break;
		}
		char dlFileName[100];
		bzero(dlFileName,100);
		strcpy(dlFileName,sendbuff);
    	strcat(dlFileName,"_dowload");
		long recvSize = 0;
		//Nhan du lieu tu server
		while(1){
		    memset(recvbuff, 0, 256);
		    rd = read(sockfd,recvbuff,255);
		    if (rd < 0) printf("Khong the doc data tu socket\n");
		    recvbuff[rd] = '\0';
	     
	     
	    	if(recvbuff[0] == '2'){
				fwrite (&recvbuff[1], 1,254, pFile);
				recvSize += 254;
	     	}
		    if(recvbuff[0] == '1') {
		     	printf("%s\n",&recvbuff[1]);
		 		break;
		    }
		    if(recvbuff[0] == '_') {
				pFile = fopen (dlFileName, "wb" );
				if (pFile == NULL) {
			    	fputs ("File error\n",stderr);
			    	break;
		     	}
		     	printf("%s\n",&recvbuff[1]);
				continue;
		    }
		     
		    if(recvbuff[0] == '3'){
				int i,end;
				for(i = 0 ; i < 256; i++){
			    	if(recvbuff[i] == '\0') end = i;
				}
				fwrite (&recvbuff[1], 1,strlen(recvbuff) -1, pFile);
				recvSize += strlen(recvbuff) -1;	
				printf("tai file thanh cong\n");
				printf("du lieu da nhan %ld byte\n", recvSize);
				printf("Vao thu muc de xem file da dowload, file dowload co ten dang *_dowload\n\n");
				fclose(pFile);
		     	break;
		    }
		}
   }
   close(sockfd);
   return 0;
}
