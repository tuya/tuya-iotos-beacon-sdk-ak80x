/* ---------------------------------------------------------------
** File name:		gpadc.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-5-27   1.0         Created
**  2021-7-9    2.0         Modified
** ---------------------------------------------------------------
*/
#ifndef _GPADC_H
#define _GPADC_H
#include "type.h"


/***
 *  Definition of calibration parameters during burning
 */
#define CALIBRATION_ADDR_BASE         (0x3FF0)
#define CALIBRATION_ADDR_FLAG         (CALIBRATION_ADDR_BASE +0x00)
#define CALIBRATION_ADDR_ORIG_BAT     (CALIBRATION_ADDR_BASE +0x02)
#define CALIBRATION_ADDR_CALI_BAT     (CALIBRATION_ADDR_BASE +0x04)
#define CALIBRATION_ADDR_ORIG_CH0     (CALIBRATION_ADDR_BASE +0x06)
#define CALIBRATION_ADDR_CALI_CH0     (CALIBRATION_ADDR_BASE +0x08)
#define CALIBRATION_FLAG              (0x5555)
#define CALIBRATION_VBAT_VALUE        (34)
#define CALIBRATION_CH0_VALUE         (18)


/***
 *  Definition of uses constants parameters
 */
#define GPADC_DATA_MAX                (0x3FF)
#define GPADC_DATA_RFA                (0x200)
#define GPADC_DATA_MIN                (0x000)

#define GPADC_CH1_RES_DIV_COEF        (15)// 10*1/coefficien , Resistive voltage divider coefficient: coefficien= 2/3
#define GPADC_VBAT_RES_DIV_COEF       (2)

#define GPADC_CH0_PORT_BIAS_VOLTAGE   (200)//unit: mV
#define GPADC_CH1_PORT_BIAS_VOLTAGE   (300)//unit: mV
#define GPADC_BATTERY_BIAS_VOLTAGE    (400)//unit: mV 
#define GPADC_VDD_RFA                 (1200)//unit: mV

#define GPPADC_MESURED_SAMPLE_COUNT   (20)

/***
 *  Definition of gpadc register
 */
#define REG_GPADC_CTRL                 0x40120100

#define START_GPADC                   (0x80)//BIT[7]gpadc ctrl singal
#define PIN0_MODE_ULL_GPADC           (0x60)//BIT[6:5]
#define PIN1_MODE_ULL_GPADC           (0x18)//BIT[4:3]
#define EN_GPADC_ULL                  (0x04)//BIT[2]en_gpadc_ull


#define REG_MODE_GPADC_ULL   0x40120130//BIT[7:6]mode_gpadc_ull<1:0>  BIT[5:4]//temp_rfb_ull<1:0> BIT[3:0],TEMP I_sence
#define GPADC_MODE_MASK      (0xC0)
#define GPADC_MODE_SHIFT     (6)
#define TEMP_RFB_MASK        (0x30)
#define TEMP_RFB_SHIFT       (4)
#define TEMP_I_SENCE_MASK    (0x0F)
#define TEMP_I_SENCE_SHIFT   (0)


#define REG_CHANNEL_GPADC    0x4012012c//BIT[7]gpadc_channel  BIT[6:5]gpadc_ga_ull<1:0>
#define GPADC_CHANNEL_0      0x00
#define GPADC_CHANNEL_1      0x80
#define GPADC_CHANNEL_MASK   0x80
#define GPADC_CHANNEL_SHIFT  (7)

#define GPADC_GAIN_SHIFT     (5)
#define GPADC_GAIN_MASK      0x60


#define REG_GPADC_SOFT_RESET 0x40120110
#define GPADC_SOFT_RST_EN    0x02//BIT[1]

#define REG_GPADC_FLAG       0x4002006c
#define FLAG_GPADC           0x04//flag_adc

#define REG_ADC_DATA_H       REG_GPADC_FLAG//BIT[1:0] - data[9:8]
#define REG_ADC_DATA_L       0x40020068//BIT[7:0] - data[7:0]

#define GPADC_CALIBRATION    (0x00)
#define GPADC_BATTERY        (0x40)
#define TEMPERATURE          (0x80)
#define ADC_CH               (0xC0)

/***
 *  
 */
typedef enum
{
    GPADC_CH0  = 0,
    GPADC_CH1  = 1,   
}gpadc_channel_t;

typedef enum
{
    GPADC_MODE_CALIBRATION  = 0,
    GPADC_MODE_BATTERY      = 1,   
    GPADC_MODE_TEMPERATURE  = 2,
    GPADC_MODE_ADC_CH       = 3
}gpadc_mode_t;


typedef enum
{
    GPADC_GAIN_0_5 = 0,  //measurement range CH0: 0.1-2.8V ; CH1:0.15 - (VDD+0.3) ;VBAT:0.2- VDD
    GPADC_GAIN_1  = 1,   //measurement range CH0: 0.2-2.6V ; CH1:0.30 - (VDD+0.3) ;VBAT:0.4- VDD
    GPADC_GAIN_2  = 2,   //measurement range CH0: 0.8-2.0V ; CH1:1.20 - 3.0V      ;VBAT:1.6- VDD
    GPADC_GAIN_4  = 3    //measurement range CH0: 1.1-1.7V ; CH1:1.65 - 2.55V     ;VBAT:2.2-3.4V
}gpadc_gain_t;

typedef enum
{
    TEMP_RFB_0 =0,
    TEMP_RFB_1 =1,
    TEMP_RFB_2 =2,
    TEMP_RFB_3 =3
}temp_rfb_t;


void gpadc_enable(void);
void gpadc_disable(void);
void gpadc_init(gpadc_channel_t channel);
void gpadc_calibration_init(void);
void temperature_init(void);
void battery_init(void);
void gpadc_gain(gpadc_gain_t gain);
void gpadc_start(void);
void gpadc_stop(void);
uint16_t gpadc_data_get(void);
void gpadc_soft_reset(void);
uint16_t gpadc_convert(gpadc_channel_t channel,gpadc_gain_t gain,uint8_t sampling_count);
uint16_t gpadc_battery_convert(void);

#endif //_GPADC_H
