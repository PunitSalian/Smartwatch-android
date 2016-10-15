/*******************************************************************************
* File Name     : speaker.c
* Description   : api for string function
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include"strtok.h"
#include <stddef.h>

/******************************************************************************
* Function name: strtok
* Description  : breaks string str into a series of tokens using the delimiter comp.
* Arguments    : str=string,comp=delimitor
* Return value : char* (returns a pointer to the last token found in the string)
******************************************************************************/
char* strtok(char * str, const char *comp)
{
	static int pos;
	static char *s;
	int start = pos;

	if (str != NULL)
		s = str;


	int j = 0;
	while (s[pos] != '\0')
	{
		j = 0;
		while (comp[j] != '\0')
		{
			//Pos point to the next location in the string that we have to read
			if (s[pos] == comp[j])
			{
				//Replace the delimter by \0 to break the string
				s[pos] = '\0';
				pos = pos + 1;
				//Checking for the case where there is no relevant string before the delimeter.
				//start specifies the location from where we have to start reading the next character
				if (s[start] != '\0')
					return (&s[start]);
				else
				{
					// Move to the next string after the delimiter
					start = pos;
					// Decrementing as it will be incremented at the end of the while loop
					pos--;
					break;
				}
			}
			j++;
		}
		pos++;
	}
	s[pos] = '\0';
	pos = 0;

	if (s[start] == '\0')
		return NULL;
	else
		return &s[start];
}

/******************************************************************************
* Function name: substring
* Description  : breaks string into substring.
* Arguments    : temp=destination,new_char=source,i,j
* Return value : none
******************************************************************************/
void substring(char *temp, char* new_char, int i, int j)
{
	char index1 = 0;
	for (int index = i; index <= j; index++)
	{

		temp[index1] = new_char[index];
		index1++;
	}
	temp[index1] = '\0';

}