/******************************************************************************
* File Name    : rtc_hal.h
* Description  :api for thermal sensor
******************************************************************************/

#ifndef RTC_HAL_H
#define RTC_HAL_H

#include "MSSGvariable/rtcmsg.h"


/*******************************************************************************
API functions Prototypes
*******************************************************************************/
void GetTime( rtclock* rt);

void Inittimer( rtclock* rt);

void clockset();

void rtcalarm(rtclock* temp);

void alarmintdisable();

void rtcstopWatch_Init();

#endif /* Multiple inclusion protection. */