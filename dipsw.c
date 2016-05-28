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
#include "dipsw.h"

void doHelp(void)
{
	printf("Usage:\n");
	printf("dipsw \n");
	printf("return  int  (0~ 15 bit are valid). \n");
}


static int fd;
/*dipsw driver open*/
int openDipSW()
{
    // open driver
    fd = open(DIPSW_DEV_NAME, O_RDWR);

    if (fd < 0)
    {
    perror("dipsw driver open error.\n");
    return 1;
    }

    return fd;
}

int dipvalue(int dipswfd)
{
	int retvalue;
	
	read(dipswfd,&retvalue,4);
	retvalue &= 0xF;
	printf("retvalue:0x%X\n",retvalue);
	close(dipswfd);
	
	return retvalue;
}
