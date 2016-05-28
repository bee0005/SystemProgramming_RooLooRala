/*---------------------------------------------------------------------
* file			: 7segment.c
* function		: Showing date
*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "7segment.h"


#define MAX_FND_NUM 6
#define DOT_OR_DATA 0x80 // dot의 bit data 값
const unsigned short segNum[10] = // 숫자를 만들기우한 세그먼트 bit 조합
{
    0x3F, // '0'
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x27,
    0x7F,
    0x6F // '9'
};
const unsigned short segSelMask[MAX_FND_NUM] = // 6개의 세그먼트의 active 값
{
    0xFE00,
    0xFD00,
    0xFB00,
    0xF700,
    0xEF00,
    0xDF00
};
static struct termios oldt, newt;
void fndChangemode(int dir) // 사용자 terminal 입력 방향 변경
{
    if( dir == 1) // 프로그램으로 키 입력 변경
    {
        tcgetattr(STDIN_FILENO, &oldt);  // 터미널의 속성 얻어옴
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO ); //터미널 속성 변경
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 변경된 속성 즉시(TCSANOW) 적용
    }
    else // 예전 방향으로 키 입력을 돌려 놓음.
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}
int fndKbhit(void)
{
    struct timeval tv;
    fd_set rdfs; // file description set
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs); // rdfs 초기화
    FD_SET(STDIN_FILENO, &rdfs);  // 확인할 file description을 표준입력
// 터미널에 입력이 있는지 확인하고 바로 나옴(time 0). 결과 rdfs에 저장
    select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);   // 체크는 0 ~ STDIN_FILENO 까지.
    return FD_ISSET(STDIN_FILENO, &rdfs);  // rdfs로 부터 표준입력이 있는지 확인
}

#define ONE_SEG_DISPLAY_TIME_USEC 1000
// return 1 => exit , 0 => success
// driverfile은 fnd file handler
// num => 표시 숫자 , dotflag => bit 별 동작 , 1 이면 dot on ,
// durationSec => 표시시간r
int fndDisp(int driverfile, int num, int dotflag,int durationSec)
{
    int cSelCounter,loopCounter;
    int temp, totalCount, i ;
    unsigned short wdata;
    int dotEnable[MAX_FND_NUM];
    int fndChar[MAX_FND_NUM];
    for (i = 0; i < MAX_FND_NUM ; i++ )
    {
        dotEnable[i] = dotflag & (0x1 << i);
    }
// if 6 fnd , 각 세그먼트별 표시 문자 파싱
    temp = num % 1000000;
    fndChar[0]= temp /100000;
    temp = num % 100000;
    fndChar[1]= temp /10000;
    temp = num % 10000;
    fndChar[2] = temp /1000;
    temp = num %1000;
    fndChar[3] = temp /100;
    temp = num %100;
    fndChar[4] = temp /10;
    fndChar[5] = num %10;
// 표시 시간 계상
    totalCount = durationSec*(1000000 / ONE_SEG_DISPLAY_TIME_USEC);
    //printf("totalcounter: %d\n",totalCount);
    cSelCounter = 0;
    loopCounter = 0;
    while(1) // 순차적으로 0 ~ 5까지의 세그먼트를 선택하면서 해당 문자 표시
    {
        wdata = segNum[fndChar[cSelCounter]] | segSelMask[cSelCounter] ;
        if (dotEnable[cSelCounter])
            wdata |= DOT_OR_DATA;
        write(driverfile,&wdata,2); // driver에 write
        cSelCounter++;
        if ( cSelCounter >= MAX_FND_NUM )
            cSelCounter = 0;
        usleep(ONE_SEG_DISPLAY_TIME_USEC);
        loopCounter++;
        if ( loopCounter > totalCount )
            break;

        if (fndKbhit()) // exit에 대비 입력문자 체크
        {
            if ( getchar() == (int)'q') // 'q' 문자 입력시 바로 exit
            {
                wdata= 0;
                write(driverfile,&wdata,2);
                printf("Exit fndtest\n");
                return 0;
            }
        }
    }
    wdata= 0;
    write(driverfile,&wdata,2);
    return 1;
}

static int fd;

int open7segment()
{
    // open driver
    fd = open(SEGMENT_DEV_NAME, O_RDWR);
    if (fd < 0)
    {
    perror("segment driver open error.\n");
    return 1;
    }

    return fd;
}

void showtime(int fd){
	int durationtime = 1;


	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int number;
		fndChangemode(1);

		number = timeinfo->tm_hour * 10000;
		number += (timeinfo->tm_min) * 100;
		number += (timeinfo->tm_sec);
		//printf("time : %d\n", number);

		fndDisp(fd, number, 0b1010, durationtime);
}