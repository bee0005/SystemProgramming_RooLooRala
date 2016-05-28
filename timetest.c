#include <stdio.h>
#include <time.h>

int main()

{
	int number;
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);



		number = timeinfo->tm_hour * 10000;
		number += timeinfo->tm_min * 100;
		number += timeinfo->tm_sec;

		printf("%d\n",number);
}