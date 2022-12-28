/*
 * @Author: zze
 * @email: zhaoze@tuya.com
 * @LastEditors: zhaoze
 * @file name: bp5758d IIC driver include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-09-17 14:58
 * @LastEditTime: 2020-09-17 14:58
 */

#ifndef __TY_LIGHT_DRIVER_BP5758D_H__
#define __TY_LIGHT_DRIVER_BP5758D_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "ty_light_basis_types.h"


/**
 * @brief bp5758d send data structure
 * sm1658cj send data structure
 */
typedef struct 
{
    u16 usRed;       ///< R value,rang from 0 to 1000
    u16 usGreen;       ///< G value,rang from 0 to 1000
    u16 usBlue;       ///< B value,rang from 0 to 1000
    u16 usCold;       ///< C value,rang from 0 to 1000
    u16 usWarm;       ///< W value,rang from 0 to 1000
}BP5758D_LIGHT_CONTRL_T;
/**
 * @brief: opUserBP5758DInit
 * @param {bp5758d_init_t* sm1658cj_init}
 * @retval: OPERATE_LIGHT
 */
OPERATE_LIGHT ty_light_driver_bp5758d_init(u8 r_pin_num, u8 g_pin_num, u8 b_pin_num, u8 c_pin_num, u8 w_pin_num, u8 color_current, u8 white_current);
/**
 * @brief: bp5758d send RGBCW function
 * @param {bp5758d_color_t *color_data}
 * @retval: OPERATE_LIGHT
 */
OPERATE_LIGHT ty_light_driver_bp5758d_rgbcw(BP5758D_LIGHT_CONTRL_T *color_data);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __BP5758D_H__ */




















































