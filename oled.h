#ifndef __OLED_H__
#define __OLED_H__


#define OLED_DEV_NAME	"/dev/cnoled"

int openOled();
void oleddoHelp(void);
int oledtest(int Mode);
void setfd(int setfd);

#endif