
/*---------------------------------------------------------------------
* ÆÄÀÏ			: keytest.c
* ±â´É			: »ç¿ëÀÚÀÇ ÀÓ°èÄ¡¸¦ ¼³Á¤
* °³¹ßÀÚ		: 201323153 ÀÌ½Â¿ø
*----------------------------------------------------------------------*/

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
#include "keypad.h"






/*    

  ### ¿ø·¡ Å°ÆÐµå¹øÈ£###
   [1] [2] [3] [4]
   [5] [6] [7] [8]
   [9] [10][11][12]
   [13][14][15][16]
   [17][18][19][20]           

   ### Å°ÆÐµå ¼öÁ¤ ###
   [1]      [2]   [3]    [x]
   [4]      [5]   [6]    [x]
   [7]      [8]   [9]    [x]
   [Log In] [0]   [È®ÀÎ] [x]
   [x]      [x]   [x]    [x]
    
   ¿ø·¡¹öÆ° ±âÁØ
   [1]~[3], [5]~[7], [9]~[11], [14]: 3X3 Çà·Ä([14]=0À» Æ÷ÇÔ)·Î ¼ýÀÚÆÇÀ¸·Î »ç¿ë
   [13] : Log In ±â´É
   [15] : ¼³Á¤°ª ÀúÀå
   [4][8][12][16][17][18][19][20] : »ç¿ë¾ÈÇÔ
   */



static int fd;

/*keypad µå¶óÀÌ¹ö open*/
int openKeypad()
{
    // open driver
    fd = open(KEYPAD_DEV_NAME, O_RDWR);

	//¾È¿­¸®¸é ¿¡·¯ Ãâ·Â
    if (fd < 0)
    {
    perror("key driver open error.\n");
    return 1;
    }

    return fd;
}

int getKeyNum(int key_fd)
{
    int rdata;

TEST:
    read(key_fd, &rdata, 4);
    	
    	if(rdata >=5 && rdata <= 7) rdata = rdata -1; //4,5,6 ¸¸µé¾îÁÖ±âÀ§ÇØ 5~7¼ýÀÚ ÀÔ·Â¹ÞÀ¸¸é -1 ÇØÁØ´Ù.
   	if(rdata >=9 && rdata <= 11) rdata = rdata -2; //7,8,9 ¸¸µé¾îÁÖ±âÀ§ÇØ 9~11 ÀÔ·Â¹ÞÀ¸¸é -2 ÇØÁØ´Ù.
   	if(rdata == 4 || rdata == 8 || rdata == 12 || rdata == 16 ) rdata = -1;
   	if(rdata == 0) goto TEST;

    //printf("button NO:%d\n", rdata);

    return rdata;
}




