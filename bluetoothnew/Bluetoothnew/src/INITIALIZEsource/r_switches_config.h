
/***********************************************************************************************************************
* File Name    : r_switches_config.c
* Description  : Configures the switches code
************************************************************************************************************************/
#ifndef SWITCHES_CONFIG_HEADER_FILE
#define SWITCHES_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* The definition for these macros should be the name of the function that you want want called when a switch is 
   pressed. It is very important that the user recognize that this function will be called from  the interrupt service 
   routine. This means that code inside of the function should be kept short to ensure it does not hold up the rest of 
   the system. 

   Example: If SW1_CALLBACK_FUNCTION is defined to be sw1_callback then the sw1_callback function will be called when 
   switch 1 is pressed.   
*/
#define SW1_CALLBACK_FUNCTION       (sw1_callback)
#define SW2_CALLBACK_FUNCTION       (sw2_callback)
#define SW3_CALLBACK_FUNCTION       (sw3_callback)

#endif /* SWITCHES_CONFIG_HEADER_FILE */
