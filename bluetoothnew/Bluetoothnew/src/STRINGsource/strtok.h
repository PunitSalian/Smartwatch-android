/******************************************************************************
* File Name    : strok.h
* Description  : api for string operation 
******************************************************************************/

#ifndef STRTOK_H
#define STRTOK_H

/******************************************************************************
Public functions prototypes
******************************************************************************/
char* strtok(char * str,const char *comp);
void substring(char *temp,char* new_char,int i, int j);
#endif