



#ifndef RTC_H
#define RTC_H

#include"MSSGvariable/rtcmsg.h"


/******************************************************************************
Public functions prototypes
******************************************************************************/
void fillstruct(int index,void* value,rtclock* r);

void DisplayTime(void);

void parsertc(char*new_char);



#endif