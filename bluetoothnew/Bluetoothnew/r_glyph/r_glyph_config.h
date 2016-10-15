
/***********************************************************************************************************************
* File Name	   : r_glyph_config.h

* Description  : Configures Glyph hook code.
***********************************************************************************************************************/


#ifndef GLYPH_CONFIG_HEADER_FILE
#define GLYPH_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
**********************************************************************************************************************/
/* Choose which RSPI channel should be used to communicate with the LCD. */
#define GLYPH_RSPI_CHANNEL      (0)

/* The Process ID to use when locking the RSPI peripheral. */
#define GLYPH_RSPI_PID          (0x474C5950)

/* If the code cannot get a lock on the RSPI peripheral then there is an option to yield the current task if a RTOS
   is present. This define specifies how many milliseconds to yield. */
#define GLYPH_LOCK_DELAY_MS     (1)

#endif /* GLYPH_CONFIG_HEADER_FILE */
