#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "buzzer.h"

#define MAX_BUZZER_NUMBER		36

static int fd;
/*buzzer driver open*/
int openBuzzer()
{
    // open driver
    fd = open(BUZZER_DEV_NAME, O_RDWR);

    if (fd < 0)
    {
    perror("buzzer driver open error.\n");
    return 1;
    }

    return fd;
}
void buzzerdoHelp(void)
{
	printf("Usage:\n");
	printf("buzzertest <buzzerNo> \n");
	printf("buzzerNo: \n");
	printf("do(1,13,25,37) ,do#(2,14,26)");
	printf("re(3,15,27), re#(4,16,28)\n");
	printf("mi(5,17,29)\n");
	printf("fa(6,18,30),fa#(7,19,31)\n");  
	printf("sol(8,20,32) ,sol#(9,21,33)\n");
	printf("ra(10,22,34), ra#(11,23,35)\n");
	printf("si(12,24,36)\n");  
}
void buttonBuzzer(int fd){
	int buzzernumber;
				buzzernumber = 6;
				write(fd,&buzzernumber,4); //파솔라파도 라솔도솔파레
				usleep(300000);
				buzzernumber = 8;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 10;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 8;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 8;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 13;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 13;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 13;
				write(fd,&buzzernumber,4);
				usleep(300000);
				buzzernumber = 10;
				write(fd,&buzzernumber,4);
				usleep(300000);
				write(fd, 0, 4);
}
