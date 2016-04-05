#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char** argv) {
    int max = -1;
    int mb = 0;
    char* buffer;
    char* buffer1;
    char* buffer2;
    char* buffer3;
    unsigned int miliseconds=1000;
    int i = 1;
    srand(time(NULL));
    int r = rand();
    if(argc > 1)
        max = atoi(argv[1]);
        miliseconds = atoi(argv[1]);
    miliseconds = 0.5;
    printf("max= %d, wait=%d\n",max,miliseconds);
    buffer = malloc(1024*1024*1024);
    memset(buffer,0,1024*1024*1024);

    buffer1 = malloc(1024*1024*1024);
    memset(buffer1,0,1024*1024*1024);

    buffer2 = malloc(1024*1024*1024);
    memset(buffer2,0,1024*1024*1024);

    buffer3 = malloc(1024*1024*1024);
    memset(buffer3,0,1024*1024*1024);
    while (i <= 50){
	r = rand();
        printf("r=%d, i=%d",r,i);    	
    	sleep(miliseconds);
    	memset(buffer,r,1024*1024*1024);
	memset(buffer1,r,1024*1024*1024);
        memset(buffer2,r,1024*1024*1024);
   	memset(buffer3,r,1024*1024*1024);
	i++;
    }
    return 0;
}

