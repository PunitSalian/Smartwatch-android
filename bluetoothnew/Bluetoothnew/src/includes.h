/*******************************************************************************
* File Name     : include.h
* Description   :headers for the main application.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <machine.h>
#include <string.h>
#include "platform.h"
#include "UARTsource/uart.h"
#include "CMTTIMERsource/cmt_periodic_multi.h"
#include "INITIALIZEsource/r_switches.h"
#include "accelerometersource/accelerometer_demo.h"
#include "RTCsource/stopwatch.h"
#include "TEMPTsource/thermal_sensor_demo.h"
#include "MSSGvariable/rtcmsg.h"
#include "RTCsource/rtc.h"
#include "GAME/topgun.h"
#include "STRINGsource/strtok.h"
#include "RTCsource/alarm.h"
#include "ADCsource/ADC.h"


#include "RTCsource/rtc_hal.h"


#define TIMER_COUNT_50MS 4688 
#define TIMER_COUNT_2MS   188 
#define TIMER_COUNT_100MS 9375 
#define TIMER_COUNT_500MS 47063 
#define TIMER_COUNT_700MS 65533
 
#define CHANNEL_0   0
#define CHANNEL_1   1
#define CHANNEL_2t   2
#define CHANNEL_3   3


#endif