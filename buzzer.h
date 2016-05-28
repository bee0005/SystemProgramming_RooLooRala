#ifndef __BUZZER_H__
#define __BUZZER_H__

#define BUZZER_DEV_NAME "/dev/cnbuzzer"

void buttonBuzzer(int fd);
void buzzerdoHelp(void);
int openBuzzer();
#endif