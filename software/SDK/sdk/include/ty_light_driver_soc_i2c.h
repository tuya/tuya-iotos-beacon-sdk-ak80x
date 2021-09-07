/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  ty_light_driver_soc_i2c.h
 * @Description: soc iic include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-10-21 14:52:06
 */
#ifndef __TY_LIGHT_DRIVER_SOC_I2C_H__
#define __TY_LIGHT_DRIVER_SOC_I2C_H__

#include "ty_light_basis_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#pragma pack(1)
/**
 * @brief IIC gpio structure
 */
typedef struct 
{
    u16 ucSdaPin;
    u16 ucSclPin;
}I2C_PIN_T;
#pragma pack()
/**
 * @brief: SOC i2c init
 * @param {IN I2C_PIN_T * pI2CPinCfg --> i2c init gpio config}
 * @retval: OPERATE_LIGHT
 */
OPERATE_LIGHT ty_light_driver_soc_i2c_init(I2C_PIN_T *pI2CPinCfg);

/**
 * @brief: SOC i2c SDA reset proc
 * @param {none}
 * @retval: none
 */
void ty_light_driver_soc_i2c_sda_reset(void);

/**
 * @brief: SOC i2c SDA set proc
 * @param {none}
 * @retval: none
 */
void ty_light_driver_soc_i2c_sda_set(void);

/**
 * @brief: SOC i2c SCL reset proc
 * @param {none}
 * @retval: none
 */
void ty_light_driver_soc_i2c_scl_reset(void);

/**
 * @brief: SOC i2c SCL set proc
 * @param {none}
 * @retval: none
 */
void ty_light_driver_soc_i2c_scl_set(void);

/**
 * @brief: SOC i2c SDA(input) read
 * @param {none}
 * @retval: none
 */
u8 ty_light_driver_soc_i2c_sda_read(void);

/**
 * @brief: sm2135 write data proc
 * @param {*pBuffer -> write data buf}
 * @param {NumByteToWrite -> write data len}
 * @retval: OPERATE_LIGHT 
 */
OPERATE_LIGHT ty_light_driver_soc_i2c_write_page(u8 *pBuffer, u16 usNumByteToWrite);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_I2C_H__ */
