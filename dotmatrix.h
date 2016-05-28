#ifndef __DOTMATRIX_H__
#define __DOTMATRIX_H__

#define DOTMATRIX_DEV_NAME "/dev/cnmled"

int openDotmatrix();
void dotChangemode(int dir);
int dotMatrixKbhit(void);
int displayDotLed(int driverfile, int num, int timeS);


#endif