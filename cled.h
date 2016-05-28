#ifndef __CLED_H__
#define __CLED_H__

#define CLED_DEV_NAME		"/dev/cncled"

int openCled();
void cleddoHelp(void);
void showcolor(int first,int second,int third, int last,int fd);

#endif