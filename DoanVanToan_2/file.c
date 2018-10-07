/* fread example: read an entire file */
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 100

int main () {
    char fileName[100];
    FILE * pFile;
    long lSize;
    char buffer[BLOCK_SIZE+1];
    size_t result;
    int  blockSize;
    
    printf("Nhap ten file:");
    scanf("%s",fileName);
    pFile = fopen ( fileName , "rb" );
    if (pFile==NULL) {
        fputs ("File error",stderr);
        exit (1);
    }
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    int sizeToRead = lSize;
    printf("The content of the file is:\n");
    while(sizeToRead > 0){
        if (sizeToRead >= BLOCK_SIZE){
            blockSize = BLOCK_SIZE;
        }
        else{
            blockSize = sizeToRead;
        }
        result = fread (buffer, 1, blockSize, pFile);
        buffer[result] = '\0';
        printf("%s",buffer);
        sizeToRead -= blockSize;
    }
    printf("\n");
    printf("The size of the file is:%ld\n",lSize);
    
    fclose (pFile);
    return 0;
}
