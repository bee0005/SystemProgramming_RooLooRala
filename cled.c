

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
#include "cled.h"


void cleddoHelp(void)
{
	printf("Usage:Color LED\n");
	printf("cledtest ledindex  red green blue\n");
	printf("color range 0 ~ 255\n");
	printf("ex) cledtest 0  255 255 255 \n");
	printf(" =>0th led is white color \n");
	printf("ex) cledtest 0 255 0 0 \n");
	printf(" =>0th led is Red color \n");
	printf("ledindex : 0 ~ 3 \n");
}
static int fd;
/*oled driver open*/
int openCled()
{
    // open driver
    fd = open(CLED_DEV_NAME, O_RDWR);

    if (fd < 0)
    {
    perror("cled driver open error.\n");
    return 1;
    }

    return fd;
}


#define INDEX_LED		0
#define INDEX_REG_LED		1
#define INDEX_GREEN_LED		2
#define INDEX_BLUE_LED		3
#define INDEX_MAX			4
void showcolor(int first,int second,int third, int last,int fd)
{
	unsigned short colorArray[INDEX_MAX];
	
	/*
	if (argc < 5 )
	{
		perror(" Args number is less than 4\n");
		doHelp();
		return 1;
	}
	
	
	colorArray[INDEX_LED] =(unsigned short) atoi(argv[1]);
	if (colorArray[INDEX_LED] > 3 )
	{
		doHelp();
		perror("led index  over range.\n");
		return 1;
	}
	colorArray[INDEX_REG_LED] =(unsigned short) atoi(argv[2]);
	colorArray[INDEX_GREEN_LED] =(unsigned short) atoi(argv[3]);
	colorArray[INDEX_BLUE_LED] =(unsigned short) atoi(argv[4]);
	
	*/

	colorArray[INDEX_REG_LED] =(unsigned short) second;
	colorArray[INDEX_GREEN_LED] =(unsigned short) third;
	colorArray[INDEX_BLUE_LED] =(unsigned short) last;
	
	// open  driver 

	//printf("index(%d) r(%d) g(%d) b(%d)\n",colorArray[INDEX_LED],colorArray[INDEX_REG_LED],colorArray[INDEX_GREEN_LED],colorArray[INDEX_BLUE_LED]);
	write(fd,&colorArray,6);

	return;
}
