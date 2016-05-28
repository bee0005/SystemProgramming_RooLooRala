#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "dotmatrix.h"
#include "7segment.h"
#include "buzzer.h"
#include "keypad.h"
#include "cled.h"
#include "oled.h"
#include "dipsw.h"

#define BAUDRATE B115200
#define ARDUINO_DEV_NAME "/dev/ttyACM0"
#define BUFFERSIZE 255

pthread_t thread_id[10];

char nettemp[BUFFERSIZE] = {0,};

int connectArduino( int *arduino_fd);
void *thrDotmatrix(void*);
void *thr7SEGMENT(void*);
void *thrKEYPAD(void*);
void *thrBUZZER(void*);
void *thrColorLed(void*);
void *thrOLED(void*);

void init();
void start();
void oledchange();

int arduino_fd = 0;
int buzzerfd;
int oledfd;
int cledfd;
int segmentfd;
int dotfd;
int keyfd;
int diqswfd;


int temp_value;
int humi_value;
int metan;
int button_value;
int vib_th = 0;
int metanstatus = 1; //1:clean 2: dirty


pthread_mutex_t cled_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cled_cond = PTHREAD_COND_INITIALIZER;


void main(void)
{
	init();
	start();
}


void init(){
	printf("INIT START!!\n");

	oledfd = openOled();
	buzzerfd = openBuzzer();
	dotfd = openDotmatrix();
	segmentfd = open7segment();
	cledfd = openCled();
	diqswfd = openDipSW();
	keyfd = openKeypad();


	oledchange();

}

void oledchange(){
	setfd(oledfd);
	oledtest(0);
	usleep(1000);
	oledtest(2);
	usleep(1000);
	oledtest(1);
}

void metanStatusCheck(){
	int first,last;
	first = metanstatus;
	if(metan > 500)
		metanstatus = 2;
	else 
		metanstatus = 1;
	last = metanstatus;

	if (first != last){
		printf("metanstatus is changed\n");
		pthread_cond_signal(&cled_cond);
		return;
	}
	
}
void start(){

	pthread_create(&thread_id[0], NULL, thrDotmatrix, NULL);
	pthread_detach(thread_id[0]);
	pthread_create(&thread_id[1], NULL, thr7SEGMENT, NULL);
	pthread_detach(thread_id[1]);
	pthread_create(&thread_id[2], NULL, thrBUZZER, NULL);
	pthread_detach(thread_id[2]);
	pthread_create(&thread_id[3], NULL, thrKEYPAD, NULL);
	pthread_detach(thread_id[3]);
	pthread_create(&thread_id[4], NULL, thrColorLed, NULL);
	pthread_detach(thread_id[4]);
	//pthread_create(&thread_id[5], NULL, thrOLED, NULL);
	//pthread_detach(thread_id[5]);
//	pthread_create(&thread_id[6], NULL, thrColorLed, NULL);
//	pthread_detach(thread_id[6]);
	
	//sensor read start
	int check =	connectArduino(&arduino_fd);	//arduino connection

	while(1){
		//Sensor Read start
		time_t startTime = 0, endTime = 0;
		char buffer[BUFFERSIZE];
		memset(buffer,0,BUFFERSIZE);
		int readIndex = 0;
		sleep(1);
		readIndex = read(arduino_fd, buffer, BUFFERSIZE);	//read arduino value, save buffer
		int entcheck = 0;
		int cnt = 0;
		char tmp[12] = { 0, };
		int i;
		sleep(3);
		for (i = 0; i < strlen(buffer); i++) {  
			if (buffer[i] == 13 && entcheck == 0) {
				entcheck = 1;
				memset(tmp, 0, cnt);
				cnt = 0;
				continue;
			}
			if (buffer[i] == 13 && entcheck == 1) {
				break;
			}
			if (buffer[i] != 13 && buffer[i] != 10) tmp[cnt++] = buffer[i];
		}
		/////////////////////////////////////////////////////////////////////////////
		
		char Ttmp[5] = { 0, };
		char Htmp[5] = { 0, };
		char Mtmp[5] = { 0, };
		char Btmp[2] = { 0, };
		int count = 0;
		int Tcheck = 0;
		int Hcheck = 0;
		int Mcheck = 0;
		int Bcheck = 0;

		//printf("!!!!!!!!!!!!!!!!!!!!!!!!! %s \n",tmp);
   // ex) 20T45H500G0B
		for (i = 0; i < strlen(tmp); i++) {

			if (tmp[i] == 'T') {
				Tcheck = 1;
				count = 0;
				continue;
			}
			if (Tcheck == 0) { 
				Ttmp[count++] = tmp[i];
      }//T


      if (tmp[i] == 'H') {
      	Hcheck = 1;
      	count = 0;
      	continue;
      }
      if (Tcheck == 1 && Hcheck == 0) {
         Htmp[count++] = tmp[i]; //H
     }

     if (tmp[i] == 'G') {
     	Mcheck = 1;
     	count = 0;
     	continue;
     }
     if (Tcheck == 1 && Hcheck == 1 && Mcheck == 0) {
            Mtmp[count++] = tmp[i]; //M
        }

        if (tmp[i] == 'B') break;
        if (Tcheck == 1 && Hcheck == 1 && Mcheck == 1 && Bcheck == 0) 
         Btmp[count++] = tmp[i];  //B
 }

   //To integer
 int T_val = atoi(Ttmp);
 temp_value = T_val;
 int H_val = atoi(Htmp);
 humi_value = H_val;
 int M_val = atoi(Mtmp);
 metan = M_val;
 int B_val = atoi(Btmp);
 button_value = B_val;
 
metanStatusCheck();

 printf("!!!!!!!!!!!!!!!!!!!!!!!\n");
 printf("Temperatur : %d \n", T_val);
 printf("Huminity : %d \n", H_val);
 printf("Methan : %d \n", M_val);
 printf("Button State : %d \n", B_val);
 printf("!!!!!!!!!!!!!!!!!!!!!!!\n");

 usleep(10);

}

}

/*----------------------------------------------------------------
* Developer	: 201323157 BaekSeungHyun					*
* Name		: thrDOTMATRIX							*
* Function	: thread printing realtime temperature	*
*----------------------------------------------------------------*/

void *thrDotmatrix(void* para){
	int durationTime = 1;

	while (1){
		dotChangemode(1);
		displayDotLed(dotfd, temp_value, durationTime);
		dotChangemode(0);
	}
	close(dotfd);

	return;
}

/*------------------------------------------------------
* Developer	: 201323157 BaekSeungHyun			*
* Name		: thr7SEGMENT					*
* Function	: thread printing Date			*
*------------------------------------------------------*/
void *thr7SEGMENT(void* para) {
	while (1) {
		showtime(segmentfd);
	}

	fndChangemode(0);
	close(segmentfd);
	return;
}

/*------------------------------------------------------
* Developer	: 201323157 BaekSeungHyun				*
* Name		: thrColorLed						*
* Function	: thread showing status				*
*------------------------------------------------------*/
void *thrColorLed(void* para) {


showcolor(0, 0, 255, 0, cledfd);

	while(1){
		pthread_cond_wait(&cled_cond, &cled_mutex);
		switch(metanstatus){
		//clean
		case 1: {
			showcolor(0, 0, 255, 0, cledfd);
			break   ;
		}

		//dirty
		case 2: {
			showcolor(0, 255 ,0, 0, cledfd);
			break;
		}
		}
	}

	close(cledfd);
	

}
/*------------------------------------------------------
* Developer	: 201323157 BaekSeungHyun				*
* Name		: thrOLED						*
* Function	: thread showing status				*
*------------------------------------------------------*/
void *thrOLED(void* para) {

}

/*----------------------------------------------------------------
* Developer	: 201323157 BaekSeungHyun						*
* Name		: thrBUZZER									*
* Function	: Buzzer sound								*
*----------------------------------------------------------------*/

void *thrBUZZER(void* para){

	while(1){
		switch(button_value){
		case 1:{
				//buttonBuzzer(buzzerfd);
				button_value = 0;
				break;
			}
		}
	}
	close(buzzerfd);
	return;
}

/*------------------------------------------------------
* Developer	: 201020228 KimGyuhun					*
* Name		: thrKEYPAD							*
* Function	: login								*
*------------------------------------------------------*/
void *thrKEYPAD(void* para){
	//int fdBuzzer;
	int key_num;
	int tmp = 0;
	int i, j;
	int flag[10] ={1,1,1,1,1,1,1,1,1,1};	
	//int flag0=1,flag1=1,flag2=1,flag3=1,flag4=1,flag5=1,flag6=1,flag7=1,flag8=1,flag9=1;

	while (1) {
		key_num = getKeyNum(keyfd);
		sleep(1);
		//printf("keynum : %d\n",key_num);
		if(key_num == 13){
			buttonBuzzer(buzzerfd); //버튼이 제대로 눌리면 확인음

			for (i = 0; tmp != 15; i++) { //15가 확인이기 때문에 15가 입력될때까지 
				//for문이 돔
				usleep(1000);
				tmp = getKeyNum(keyfd); //눌려진 버튼값 입력받음
				
		

				if (tmp == 13 || tmp == -1)/*|| tmp == 4 ||tmp == 8 ||tmp == 12 || tmp == 16 )*/ { //사용하지 않는값이 들어오면 무시하고 
					//다음 숫자값 읽음
					continue;
				}

				else if (flag[tmp]==1){
					// if(tmp==0) flag[0];hhhhhhh
					printf("i's value : %d", i);
					
					flag[tmp] ==0;

					vib_th = vib_th * 10 + tmp;  //사용자가 입력한숫자대로 출력해주기 위해서 
					printf("%d", vib_th);
					//자릿수를 한칸씩 옮김
				}
				if (vib_th > 999999) {//7-segment가 표현할수 있는자리수보다 많이 들어오면 99999로 에러처리
					printf("[protection]over vib_th\n");
					vib_th = 999999;
				}
			}
			printf("vib_th : %d [input success]\n", vib_th);
			vib_th = 0;

		}
	}
	close(keyfd);
}

int connectArduino(int *arduino_fd) {
	struct termios term_char;
	*arduino_fd = open(ARDUINO_DEV_NAME, O_RDWR | O_NOCTTY);
	if (*arduino_fd < 0) {
		perror(ARDUINO_DEV_NAME);
		return -1;
	}
	bzero(&term_char, sizeof(term_char));
	term_char.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	term_char.c_iflag = IGNPAR;
	term_char.c_oflag = 0;
	tcflush(*arduino_fd, TCIFLUSH);
	tcsetattr(*arduino_fd, TCSANOW, &term_char);
	printf("ArduinoCONNECTION\n");
	return 1;
}
