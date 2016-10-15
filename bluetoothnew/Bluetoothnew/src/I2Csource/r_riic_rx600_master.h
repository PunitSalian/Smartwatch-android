
/*******************************************************************************
* File Name    : r_riic_rx600_master.h
* Description  : RIIC driver master mode API function declarations header file.
********************************************************************************/

#ifndef _IIC_DEF_H
#define _IIC_DEF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_riic_rx600.h"


/******************************************************************************
Functions Prototypes
*******************************************************************************/
riic_ret_t R_RIIC_MasterTransmitHead(uint8_t          channel,                                      
                                     uint8_t *        p_data_buff, 
                                     const uint32_t   num_bytes);
riic_ret_t R_RIIC_MasterReceive(uint8_t         channel,
                                uint8_t         slave_addr,
                                uint8_t *       p_data_buff,
                                const uint32_t  num_bytes);
riic_ret_t R_RIIC_MasterTransmit(uint8_t        channel,
                                 uint8_t *      p_data_buff, 
                                 const uint32_t num_bytes);
                              
#endif /* _IIC_DEF_H */


