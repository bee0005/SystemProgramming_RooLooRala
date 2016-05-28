#ifndef __DIPSWITCH_H__
#define __DIPSWITCH_H__

#define DIPSW_DEV_NAME		"/dev/cndipsw"

int openDipSW();
int dipvalue(int dipswfd);


#endif