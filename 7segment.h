#ifndef __7SEGMENT_H__
#define __7SEGMENT_H__

#define SEGMENT_DEV_NAME "/dev/cnfnd"
#define MODE_TIME_DIS		1

void fndChangemode(int dir);
int fndKbhit(void);
int open7segment();
int fndDisp(int driverfile, int num, int dotflag,int durationSec);
void showtime(int fd);
#endif
    