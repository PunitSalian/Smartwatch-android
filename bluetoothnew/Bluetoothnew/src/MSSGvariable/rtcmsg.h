#ifndef RTCMSG_H
#define RTCMSG_H

#include<stdbool.h>

typedef struct msb
{
bool messageflag;
bool callflag;
bool timeflag;
bool detailflag;	
}message;

typedef struct timedate
{
char   Mon1,Mon10,Day1,Day10, Hour1,Hour10,Min1,Min10,Sec1,Sec10,Day;
unsigned short Year1,Year10;
char* dur;
}rtclock;

#endif