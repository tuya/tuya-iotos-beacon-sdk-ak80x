#include "gpadc.h"
#include "delay.h"
#include "type.h"
#include "sys.h"
#include "int.h"

void gpadc_pin(gpadc_channel_t channel)
{
    uint8_t reg_val =0;
    reg_val = read_reg(REG_GPADC_CTRL);
    switch (channel)
        {
            case 0:
            {
                reg_val &= ~PIN0_MODE_ULL_GPADC;
                break;
            }
            case 1:
            {
                reg_val &= ~PIN1_MODE_ULL_GPADC;
            }
            default:
            {
                break;
            }
        }
    write_reg(REG_GPADC_CTRL, reg_val);    
}

void gpadc_mode(gpadc_mode_t mode)
{
    uint32_t reg_val =0;
    reg_val =read_reg(REG_MODE_GPADC_ULL);//default value:0x28 ,gpadc mode: calibration
    reg_val &= ~GPADC_MODE_MASK;
    reg_val |= mode<<GPADC_MODE_SHIFT;
    
    write_reg(REG_MODE_GPADC_ULL, reg_val);
}
void temperature_para(temp_rfb_t temp_rfb,uint8_t i_sence)
{
    uint32_t reg_val =0;
    /*Set the TEMP_RFB parameter */
    reg_val = read_reg(REG_MODE_GPADC_ULL);//default value:0x28 ,temp_rfb = 2 i_sence = 8
    reg_val &=~TEMP_RFB_MASK;
    reg_val |=temp_rfb;
    
    /*Set the TEMP_I_SENCE parameter */
    reg_val &=~TEMP_I_SENCE_MASK;
    reg_val |=(TEMP_I_SENCE_MASK & i_sence);
        
    write_reg(REG_MODE_GPADC_ULL, reg_val); 
}

void gpadc_channel(gpadc_channel_t channel)
{
    uint32_t reg_val =0;
    reg_val =read_reg(REG_CHANNEL_GPADC);
    reg_val &= ~GPADC_CHANNEL_MASK;
    reg_val |= channel<<GPADC_CHANNEL_SHIFT;
    write_reg(REG_CHANNEL_GPADC, reg_val);
}

void gpadc_gain(gpadc_gain_t gain)
{
    uint32_t reg_val =0;
    reg_val =read_reg(REG_CHANNEL_GPADC);//default values:0x50 ,gain 2
    reg_val &= ~GPADC_GAIN_MASK;
    reg_val =gain<<GPADC_GAIN_SHIFT;
    write_reg(REG_CHANNEL_GPADC, reg_val);
}
void gpadc_enable(void)
{
    uint8_t reg_val =0;
    reg_val =read_reg(REG_GPADC_CTRL);

    reg_val |= EN_GPADC_ULL;

    write_reg(REG_GPADC_CTRL, reg_val);
    delay_us(2);
}
void gpadc_disable(void)
{
    uint8_t reg_val =0;
    reg_val =read_reg(REG_GPADC_CTRL);

    reg_val &= ~EN_GPADC_ULL;

    write_reg(REG_GPADC_CTRL, reg_val);
    delay_us(2);
}

void gpadc_init(gpadc_channel_t channel)
{
    gpadc_pin(channel);
    gpadc_mode(GPADC_MODE_ADC_CH);
    gpadc_channel(channel);
    gpadc_enable();
}

void temperature_init(void)
{
    gpadc_mode(GPADC_MODE_TEMPERATURE);
    gpadc_enable();
    delay_us(2);
}

void battery_init(void)
{
    gpadc_mode(GPADC_MODE_BATTERY);
    gpadc_enable();
    delay_us(2);
}
void gpadc_calibration_init(void)
{
    gpadc_mode(GPADC_MODE_CALIBRATION);
    gpadc_enable();
    delay_us(2);
}

void gpadc_start(void)
{
    uint8_t reg_val =0;
    reg_val =read_reg(REG_GPADC_CTRL);

    reg_val |=START_GPADC;

    write_reg(REG_GPADC_CTRL, reg_val);
}
void gpadc_stop(void)
{
    uint8_t reg_val =0;
    reg_val =read_reg(REG_GPADC_CTRL);

    reg_val &=~START_GPADC;

    write_reg(REG_GPADC_CTRL, reg_val);

}
uint16_t gpadc_data_get(void)
{
    uint16_t result =0;
    if(FLAG_GPADC & read_reg(REG_GPADC_FLAG))
    {
        result  = 0x03 & read_reg(REG_ADC_DATA_H);
        result = result << 8;    
        result |= (0xFF & read_reg(REG_ADC_DATA_L));
    }
    return result;
}
 void gpadc_soft_reset(void)
{
    uint8_t reg_val =0;
    reg_val = read_reg(REG_GPADC_SOFT_RESET);
    reg_val |= GPADC_SOFT_RST_EN;

    write_reg(REG_GPADC_SOFT_RESET, reg_val);
    delay_us(10);
    reg_val &= ~GPADC_SOFT_RST_EN;
    write_reg(REG_GPADC_SOFT_RESET, reg_val);
}

 
 /*****************************************************************************************
  * @function  gpadc ch0/1 ADC convert 
  *
  * @param[in] channel         Select the GPADC channel CH0 or CH1
  * @param[in] gain            Choose ADC gain, Optional: 0.5/1/2/4
  * @param[in] sampling_count  Average number of samples filtered
  * @return    Signal acquisition results are converted into voltage output in mV
  *****************************************************************************************/

uint16_t gpadc_convert(gpadc_channel_t channel,gpadc_gain_t gain,uint8_t sampling_count)
{
    uint32_t gpadc_deviation_value =0;
    int32_t gpadc_value =0;
    gpadc_disable();
    gpadc_calibration_init();
    gpadc_gain(gain);
    gpadc_soft_reset();
    if(0 == sampling_count)
    {
        sampling_count =1;
    }
    for(uint8_t i=0;i<sampling_count;i++)
    {
        gpadc_soft_reset();
        delay_us(2);
        gpadc_start();

        gpadc_deviation_value += gpadc_data_get();
    }
    gpadc_deviation_value = gpadc_deviation_value/sampling_count;
    gpadc_disable();
    gpadc_init(channel);
    gpadc_soft_reset();

    for(uint8_t i=0;i<sampling_count;i++)
    {
        gpadc_soft_reset();
        delay_us(2);
        gpadc_start();
        gpadc_value += gpadc_data_get();
    }
    gpadc_value = gpadc_value/sampling_count;

    gpadc_value = gpadc_value +(GPADC_DATA_RFA - gpadc_deviation_value);//

    gpadc_value = GPADC_VDD_RFA*(gpadc_value - GPADC_DATA_RFA)/GPADC_DATA_RFA;

    switch (gain)
    {
        case GPADC_GAIN_0_5:
        {
            gpadc_value = gpadc_value<<1;
            break;
        }
        case GPADC_GAIN_1:
        {
            gpadc_value = gpadc_value;
            break;
        }
        case GPADC_GAIN_2:
        {
            gpadc_value = gpadc_value>>1;

            break;
        }
        case GPADC_GAIN_4:
        {
            gpadc_value = gpadc_value>>2;
            break;
        }
        default:
        {
            gpadc_value = gpadc_value>>1;
            break;
        }
    }
    if(GPADC_CH0 == channel)
    {
        gpadc_value =gpadc_value + GPADC_VDD_RFA + GPADC_CH0_PORT_BIAS_VOLTAGE;
    }
    else if(GPADC_CH1 == channel)
    {
        gpadc_value = (gpadc_value + GPADC_VDD_RFA)*GPADC_CH1_RES_DIV_COEF/10 + GPADC_CH1_PORT_BIAS_VOLTAGE;
        
    }

    return (gpadc_value & 0xFFFF);
}
 /*****************************************************************************************
  * @function  gpadc battery convert 
  * @param[in] 
  * @return    Signal acquisition results are converted into voltage output in mV
  *****************************************************************************************/

uint16_t gpadc_battery_convert(void)
{
    uint32_t gpadc_deviation_value =0;
    uint32_t gpadc_value =0;

    /*1.gpadc calibration init*/
    gpadc_disable();
    gpadc_gain(GPADC_GAIN_1);
    gpadc_calibration_init();
    gpadc_soft_reset();

    /*2.get calibration convert result*/
    delay_us(2);
    gpadc_start();

    gpadc_deviation_value = gpadc_data_get();
    
    delay_us(10);
    
    /*3. select gpadc mode at battery mode*/
    gpadc_disable();
    battery_init();
    gpadc_soft_reset();

    /*4. get battery convert result */
    delay_us(2);
    gpadc_start();
    gpadc_value = gpadc_data_get();

    /*5. convert into battery voltage according to the formula*/
    gpadc_value = gpadc_value +(GPADC_DATA_RFA - gpadc_deviation_value);//


    gpadc_value = GPADC_VDD_RFA*(gpadc_value - GPADC_DATA_RFA)/GPADC_DATA_RFA;

    
    gpadc_value = (gpadc_value + GPADC_VDD_RFA )*GPADC_CH1_RES_DIV_COEF*GPADC_CH1_RES_DIV_COEF/100 + GPADC_BATTERY_BIAS_VOLTAGE;//


    return (gpadc_value & 0xFFFF);
}
