
/***********************************************************************************************************************
* File Name     : r_rspi_rx600_config.c
* Description   : Configures the RSPI drivers
************************************************************************************************************************/
#ifndef RSPI_CONFIG_HEADER_FILE
#define RSPI_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* If this definition is uncommented then obtaining a lock on the RSPI peripheral using the R_RSPI_Lock() function will 
   be required. After obtaining the lock the user will have to pass the tasks PID to all other RSPI functions. If this 
   is commented out then locking will not be enabled and the user can pass any value they want for 'pid' because the 
   functions will ignore the lock. */
#define RSPI_REQUIRE_LOCK

#endif /* RSPI_CONFIG_HEADER_FILE */
