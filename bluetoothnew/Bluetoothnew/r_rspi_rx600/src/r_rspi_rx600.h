
/***********************************************************************************************************************
* File Name    : r_rspi_rx600.h
* Description  : Functions for using RSPI on RX600 devices. 
************************************************************************************************************************/


#ifndef RSPI_API_HEADER_FILE
#define RSPI_API_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for configuring the RSPI code */
#include "r_rspi_rx600_config.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Enumeration for chips selects associated with YRDKRX62N RSPI-0 */
typedef enum {
    NO_DEVICE_SELECTED=0,
    SDMICRO_SELECTED,
    FLASH_SELECTED,
    WIFI_SELECTED,
    LCD_SELECTED
} device_selected_t;

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
bool R_RSPI_Init(uint8_t channel);
bool R_RSPI_Select(uint8_t channel, uint16_t chip_select, uint32_t pid);
bool R_RSPI_Deselect(uint8_t channel, uint16_t chip_select, uint32_t pid);
bool R_RSPI_BaudRateSet(uint8_t channel, uint8_t divisor, uint32_t pid);
bool R_RSPI_SendReceive(uint8_t channel, uint8_t const *pSrc, uint8_t *pDest, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Read(uint8_t channel, uint8_t *pDest, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Write(uint8_t channel, const uint8_t *pSrc, uint16_t usBytes, uint32_t pid);
bool R_RSPI_Lock(uint8_t channel, uint32_t pid);
bool R_RSPI_Unlock(uint8_t channel, uint32_t pid);
bool R_RSPI_CheckLock(uint8_t channel, uint32_t pid);

#endif /* RSPI_API_HEADER_FILE */

