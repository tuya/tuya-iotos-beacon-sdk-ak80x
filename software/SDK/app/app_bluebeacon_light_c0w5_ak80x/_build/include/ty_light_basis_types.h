/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: ty_light_basis_types.h
 * @Description: light control support function include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-10-18 10:53:39
 */

#ifndef __TY_LIHGT_BASIS_TYPES_H__
#define __TY_LIHGT_BASIS_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "board.h"
//#include "hal_log.h"
#include "hal_gpio.h" 

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

typedef int OPERATE_LIGHT;


typedef enum {
    DEF_COLOR_C = 0,
    DEF_COLOR_W,
    DEF_COLOR_R,
    DEF_COLOR_G,
    DEF_COLOR_B,
    DEF_COLOR_RGB,
    DEF_COLOR_MAX,
}CTRL_DEF_COLOR_E;

typedef enum
{
    WHITE_MODE = 0,
    COLOR_MODE,
    SCENE_MODE,
    MUSIC_MODE,
    MODE_MAX,
}LIGHT_MODE_E;

/**
 * @brief Light control drive mode enum
 *          BRIGHT_MODE_CW  -> CW drive by pwm totally
 *          BRIGHT_MODE_CCT -> C value is bright setting essentially , w is the scale of C&W .
 *                          the light will send warm, when w is set by zero.
 */
typedef enum {
    BRIGHT_MODE_CW = 0,
    BRIGHT_MODE_CCT,
    BRIGHT_MODE_MAX,
}CTRL_BRIGHT_MODE_E;

/**
 * @brief Light control switch change mode enum 
 *          SWITCH_GRADUAL -> turn on/off gradually
 *          SWITCH_DIRECT  -> turn on/off directly
 */
typedef enum {
    SWITCH_GRADUAL = 0,
    SWITCH_DIRECT,
    SWITCH_MAX,
}CTRL_SWITCH_MODE_E;

/* enum need to define in json analysize & get file */
typedef enum {
    DRIVER_MODE_PWM = 0,
    DRIVER_MODE_SM16726B,
    DRIVER_MODE_SM2135,
    DRIVER_MODE_BP1658CJ,
    DRIVER_MODE_BP5758D,
}DRIVER_MODE_E;
#pragma pack(1)
typedef struct {
    u16 usFreq;            ///< PWM Frequency
    u16 usDuty;            ///< PWM Init duty
    u8  ucList[5];         ///< GPIO List 
    u8  ucChannelNum;      ///< GPIO List length
    bool   bPolarity;         ///< PWM output polarity
    u8  ucCtrlPin;         ///< CTRL pin parm
    bool   bCtrlLevel;        ///< Enable level
    bool   bCCTFlag;          ///< CCT drive flag
}PWM_INIT_T;

typedef struct {
    u8 ucNumber;           ///< output channel number
    u8 ucSdaPin;           ///< SDA pin
    u8 ucSclPin;           ///< SCL pin
    u8 ucList[5];          ///< RGBCW output channel index config 
    u8 ucRGBCurrent;       ///< RGB mode max current settings
    u8 ucCWCurrent;        ///< CW mode max current settings
    u8 ucCtrlPin;          ///< CTRL pin parm
    bool  bCtrlLevel;         ///< Enable level
}SM2135_INIT_T;

typedef struct {
    u16 usFreq;            ///< PWM Frequency
    u16 usDuty;            ///< PWM Init duty
    u8  ucColdPin;         ///< PWM cold output pin
    u8  ucWarmPin;         ///< pwm warm output pin          
    bool   bPolarity;         ///< PWM output polarity
    bool   bCCTFlag;          ///< PWM CCT drive flag
}SM16726B_PWM_INIT_T;

typedef struct {
    u8 ucSdaPin;                   ///< SDA pin
    u8 ucSclPin;                   ///< SCL pin
    u8 ucList[3];                  ///< R, G, B index of SM16726 output pin
    u8 ucCtrlPin;                  ///< CTRL pin parm
    bool  bCtrlLevel;                 ///< Enable level
    u8 ucChannelNum;               ///< Light channel: 3/4/5
    SM16726B_PWM_INIT_T tPwmInit;     ///< Channel C,W config
}SM16726B_INIT_T;

/**
 * @brief bp5758d init gpio pin structure
 * sm1658cj init gpio structure
 */
typedef struct 
{
    u8 ucNumber;            ///< output channel number
    u16 ucSdaPin;            ///< SDA pin
    u16 ucSclPin;            ///< SCL pin
    u8 ucList[5];           ///< RGBCW output channel index config
    u8 ucCurrentList[5];    ///< R/G/B/C/W max current list
    u8 ucCtrlPin;           ///< CTRL pin parm
    bool  bCtrlLevel;            ///< Enable level
}BP5758D_INIT_T;

/**
 * @brief bp1658cj init gpio pin structure
 * sm1658cj init gpio structure
 */
typedef struct 
{
    u8 ucNumber;            ///< output channel number
    u8 ucSdaPin;            ///< SDA pin
    u8 ucSclPin;            ///< SCL pin
    u8 ucList[5];           ///< RGBCW output channel index config 
    u8 ucRGBCurrent;       ///< RGB mode max current settings
    u8 ucCWCurrent;        ///< CW mode max current settings
    u8 ucCtrlPin;           ///< CTRL pin parm
    bool  bCtrlLevel;            ///< Enable level
}BP1658CJ_INIT_T;

typedef struct{
    DRIVER_MODE_E eMode;
    union{
        PWM_INIT_T tPwmInit;
        SM2135_INIT_T tSm2135Init;
        SM16726B_INIT_T tSm16726bInit;
        BP1658CJ_INIT_T tBp1678cjInit;
        BP5758D_INIT_T tBp5758dInit;
    } uConfig;
}DRIVER_CONFIG_T;
#pragma pack()

#define TUYA_PUB_ADDR 0xD000


#define CTRL_BRIGHT_LIMIT_MAX 100
#define CTRL_TEMPER_LIMIT_MAX 100

/// control power gain (100 --> gain = 1 times )
#define CTRL_POEWER_GAIN_MIN        100 
/// control power gain (200 --> gain = 2 times )   
#define CTRL_POEWER_GAIN_MAX        200    

/// Control calculate range 0 ~ 20000
#define CTRL_CAL_VALUE_RANGE        20000
/// Control cw calc max bright value
#define CTRL_CW_BRIGHT_VALUE_MAX    1000
#define CTRL_CW_TEMPER_VALUE_MAX    1000
/// Control cw calc min bright value (max value * 0.01)
#define CTRL_CW_BRIGHT_VALUE_MIN    (CTRL_CW_BRIGHT_VALUE_MAX * 0.01) 

/// Control RGB calc max bright value
#define CTRL_RGB_BRIGHT_VALUE_MAX   1000
/// Control RGB calc min bright value
#define CTRL_RGB_BRIGHT_VALUE_MIN   (CTRL_RGB_BRIGHT_VALUE_MAX * 0.01)

#define CTRL_BEFOR_CAL_VALUE_RANGE        1000

#ifndef IN
#define IN 
#endif

#ifndef OUT
#define OUT
#endif

#ifndef LIGHT_OK
#define LIGHT_OK                             (0)
#endif
#ifndef LIGHT_COM_ERROR
#define LIGHT_COM_ERROR                      (-1)
#endif
#ifndef LIGHT_INVALID_PARM
#define LIGHT_INVALID_PARM                   (-2)
#endif
#ifndef LIGHT_MALLOC_FAILED
#define LIGHT_MALLOC_FAILED                  (-3)
#endif
#ifndef LIGHT_MORE_DATA
#define LIGHT_MORE_DATA                      (-4)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif



#ifndef DEBUG
#define TY_LOG_NOTICE
#define TY_LOG_ERR
#define TY_LOG_DEBUG
#endif

#define PIN_NOEXIST        0xFF  ///< default pin def


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_CFG_H__ */
