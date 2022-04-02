/* ---------------------------------------------------------------
** File name:		rf.c
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
**
** ---------------------------------------------------------------
*/


/* Includes -----------------------------------------------------*/
#include <stdbool.h>

#include "type.h"
#include "config.h"
#include "sys.h"
#include "rf.h"
#include "delay.h"

/* Macro --------------------------------------------------------*/

/* Global Variables ---------------------------------------------*/


/* Extern -------------------------------------------------------*/

/* Functions ----------------------------------------------------*/

/**
 * @brief  Config syncwords, Premble + Access address. Send by LSB of [sw_high+sw_low].
 * @param  channel                    channel index, 0/1. If you need 2 different syncwords, config it to 1.
 * @param  sw_high                    1 Byte. MSB of Access address. Fpr example, send BLE packets, 0x8E in 0x8E89BED6).
 * @param  sw_low                     4 Bytes.
 */
void rf_set_rx_sync_words( uint8_t channel, uint8_t sw_high, uint32_t sw_low)
{
    uint32_t    reg_val = 0;

    if ( BB_CHANNEL_0 == channel )
    {
        write_reg(BB_AC_CODE1_REG, sw_low);

        reg_val = read_reg(BB_AC_CODE_HIGHPART_REG);
        reg_val &= ~BB_CS_SYNCWORD1_HIGHPART_MASK;
        reg_val |= sw_high << BB_CS_SYNCWORD1_HIGHPART_SHIFT;
        write_reg(BB_AC_CODE_HIGHPART_REG, reg_val);
    }
#if(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_BOTH)
    else if ( BB_CHANNEL_1 == channel )
    {
        write_reg(BB_AC_CODE2_REG, sw_low);

        reg_val = read_reg(BB_AC_CODE_HIGHPART_REG);
        reg_val &= ~BB_CS_SYNCWORD2_HIGHPART_MASK;
        reg_val |= sw_high << BB_CS_SYNCWORD2_HIGHPART_SHIFT;
        write_reg(BB_AC_CODE_HIGHPART_REG, reg_val);
    }
#endif
}

/**
 * @brief  Config BaseBand RAM as FIFO or Single RAM.
 * @param  bbram_model                @ref bbram_mode_t "bbram mode."
 */
void rf_set_bbram_mode( bbram_mode_t bbram_mode )
{
    uint8_t delay_mode = 0;

    /* Set BBRam model */
    write_reg(BB_RAM_MODEL_REG, bbram_mode);

#if 1
	if((BBRAM_SINGLE_RAM_RX == bbram_mode) || (BBRAM_SINGLE_RAM_TX == bbram_mode))
	{
		delay_mode = 1;
	}
#else
    /* Set delay mode */
    switch( bbram_mode )
    {
    case    BBRAM_FIFO_RX:
    case    BBRAM_FIFO_TX:
    case    BBRAM_FIFO_TXRX:
        delay_mode = 0;
        break;

    case    BBRAM_SINGLE_RAM_RX:
    case    BBRAM_SINGLE_RAM_TX:
        delay_mode = 1;
        break;

    default:
        delay_mode = 0;
        break;
    }
#endif

    write_reg(BB_RX_GFSK_DCC_DL_MODE_REG, delay_mode);

    if ( delay_mode )
    {
        write_reg(BB_LEN_ERR_LIMIT_REG, 127);
    }
    else
    {
        write_reg(BB_LEN_ERR_LIMIT_REG, 63);
    }
}

/**
 * @brief  Config BaseBand as Tx or Rx.
 * @param  bbram_model                @ref rx_tx_mode_t "Tx and Rx mode."
 */
void rf_set_rx_tx_mode( rx_tx_mode_t rx_tx_model )
{
    write_reg(BB_RX_TX_SEL_REG, rx_tx_model);
}

void rf_set_compensate( rf_gfo_comp_t gfo_comp )
{
    write_reg(BB_GFO_COMPENSATE_REG, gfo_comp);
}

/**
 * @brief  Config Sync bits of received packets. For example, 40_BIT needs to synchronize 40 bits.
 * @param  sync_bits_mode             @ref bb_rx_syncword_mode_t "the choice of the supported sync_bits"
 */
void rf_set_rx_sync_bits(uint8_t sync_bits_mode)
{
    write_reg(BB_REG_CORR_MODE_REG, sync_bits_mode);
}

/**
 * @brief  CRC error interrupt configuration.
 * @param  on_off                     @ref bb_crc_rx_int_t "1: enable interrupt if crc error, 0: disable interrupt if crc error."
 */
void rf_set_crc_rx_int_mode( uint8_t on_off )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW1_REG );

    reg_val &= ~BB_CRCERR_MASK_MASK;
    reg_val |= on_off << BB_CRCERR_MASK_SHIFT;

    write_reg( BB_REG_BASEBAND_STATE_RW1_REG, reg_val );
}

/**
 * @brief  Config the length of RX packets.
 * @param  channel_index              0/1, Cooperate with rf_set_rx_sync_words().
 * @param  len_mode                   @ref bb_pack_len_mode_t "the supported len_mode." 
 * @param  len                        the setting of length, 0 if mode is BB_PACK_LEN_AUTO.
 */
void rf_set_rx_len_mode(        bb_pack_len_mode_t channel_0_len_mode, 
                                bb_pack_len_mode_t channel_1_len_mode )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW5_REG );

    reg_val &= ~BB_RX_LENGTH_MODE_MASK;
    reg_val |= channel_0_len_mode << 0;
    reg_val |= channel_1_len_mode << 1;

    write_reg( BB_REG_BASEBAND_STATE_RW5_REG, reg_val );
}

void rf_set_rx_len(     bb_channel_t channel_index, 
                        bb_pack_len_mode_t len_mode, 
                        uint8_t len )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW5_REG );

    if ( BB_CHANNEL_0 == channel_index )
    {
        reg_val &= ~BB_RX_CHNL0_LENGTH_MODE_MASK;
        reg_val |= len_mode << BB_RX_CHNL0_LENGTH_MODE_SHIFT;

        reg_val &= ~BB_RXLEN_CPU0_MASK;
        reg_val |= len << BB_RXLEN_CPU0_SHIFT;
    }
#if(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_BOTH)
    else if ( BB_CHANNEL_1 == channel_index )
    {
        reg_val &= ~BB_RX_CHNL1_LENGTH_MODE_MASK;
        reg_val |= len_mode << BB_RX_CHNL1_LENGTH_MODE_SHIFT;

        reg_val &= ~BB_RXLEN_CPU1_MASK;
        reg_val |= len << BB_RXLEN_CPU1_SHIFT;
    }
#endif

    write_reg( BB_REG_BASEBAND_STATE_RW5_REG, reg_val );
}

/**
 * @brief  Init the function of RF.
 */
void rf_init()
{
    uint32_t reg_val = 0, n = 400;

    /* Clock enable */
    sys_set_module_clock( CLK_BB_EN_MASK, ON );

    /* Softreset */
    sys_soft_reset( BB_SOFT_RESET_MASK );

    /* Config AA */
    rf_set_rx_sync_words( BB_CHANNEL_0, BB_SW_HIGH_1_BYTE, BB_SW_LOW_4_BYTE );
#if(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_BOTH)
	//rf_set_rx_sync_words( BB_CHANNEL_1, 0x8E, 0x35BED6AA );
#endif

    /* Active bits of sync words */
    rf_set_rx_sync_bits( EORR_MODE_ACTIVE_BITS );

    /* Open channels */
#if(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_SINGLE)
    rf_set_rx_channels( BB_CHANNEL_0_ONLY );
#elif(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_BOTH)
    rf_set_rx_channels( BB_CHANNEL_0_1 );
#endif

    /* Config compensate */
    rf_set_compensate( BB_RF_GFO_COMP );

    /* Config phase invert */
    rf_set_rx_phase_intert( FALSE );

    /* p2p THR */
    reg_val = read_reg(BB_RX_P2P_THR_REG);
    reg_val &= ~BB_TEMP_MASK;
    reg_val |= 0x156 << BB_TEMP_SHIFT;

    reg_val &= ~BB_RX_1ST_P2P_MASK;
    reg_val |= 0x156 << BB_RX_1ST_P2P_SHIFT;
    write_reg(BB_RX_P2P_THR_REG, reg_val);

    /* 0x64, CRC error raise , package length */
#if 1
    rf_set_crc_rx_int_mode( BB_CRC_RX_INT_COMBINED_CFG );
    rf_set_rx_len( BB_CHANNEL_0, BB_CHANNEL_0_PKG_LEN_MODE, BB_CHANNEL_0_PKG_LEN );
#if(ENABLE_RX_CHANNEL_NUM == RX_CHANNEL_BOTH)
    rf_set_rx_len( BB_CHANNEL_1, BB_CHANNEL_1_PKG_LEN_MODE, BB_CHANNEL_1_PKG_LEN );
#endif
#else
    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW5_REG );

    // CRC error and rx.
    reg_val &= ~BB_CRCERR_MASK_MASK;
    reg_val |= BB_CRC_RX_INT_COMBINED_CFG << BB_CRCERR_MASK_SHIFT;

    // Package length mode
    reg_val &= ~BB_RX_LENGTH_MODE_MASK;
    reg_val |= BB_PACK_LEN_MODE << BB_RX_LENGTH_MODE_SHIFT;

    // RX package length
    reg_val &= ~BB_RXLEN_CPU1_MASK;
    reg_val |= BB_CHANNEL_1_PKG_LEN << BB_RXLEN_CPU1_SHIFT;

    reg_val &= ~BB_RXLEN_CPU0_MASK;
    reg_val |= BB_CHANNEL_0_PKG_LEN << BB_RXLEN_CPU0_SHIFT;

    write_reg( BB_REG_BASEBAND_STATE_RW5_REG, reg_val );
#endif

    /* Config CRC initial value, CRC and whitening control */
    write_reg( BB_REG_BIT_STREAM_REG, (BB_CRC_INTI_VAL | BB_WHIT_CTRL | BB_CRC_CTRL));

    /* Config channel number */
    rf_set_channel_num(BB_CHANNEL_NUMB);

    reg_val = read_reg( BB_AGC_CONFIG_1_REG );
    reg_val |= 8;
    write_reg( BB_AGC_CONFIG_1_REG, reg_val );

    // agc power
    reg_val = read_reg( BB_AGC_CONFIG_1_REG );
    reg_val &= ~BB_TARGET_SIG_PWR_MASK;
    reg_val |= 1 << BB_TARGET_SIG_PWR_SHIFT;
    write_reg( BB_AGC_CONFIG_1_REG, reg_val );

    write_reg( BB_DFE_FFF_COEF_REG, 0x73FF82 );
    write_reg( BB_DFE_FBF_COEF_REG, 0x30C );

    reg_val = read_reg( BB_REG_RXGFSK_PE_REG );
    reg_val &= ~BB_PE_PWR_SQR_UN_THR_MASK;
    reg_val |= 0x70 << BB_PE_PWR_SQR_UN_THR_SHIFT;
    write_reg( BB_REG_RXGFSK_PE_REG, reg_val);

    reg_val = read_reg( BB_PE_CTRL_REG );
    reg_val &= ~0x1000000;
    write_reg( BB_PE_CTRL_REG, reg_val );

    reg_val = read_reg( BB_REG_AC_CORRELATOR_REG );
    reg_val &= ~BB_SYNCERR_THR_MASK;
    reg_val |= 2 << BB_SYNCERR_THR_SHIFT;
    write_reg( BB_REG_AC_CORRELATOR_REG, reg_val );

    write_reg(0x40070024, 0x361dff);
    /*To ensure the correctness of the first packet,tx without power amplified for a time*/
    write_reg(CORE_CTRL_BASE_ADDR,0X0F);
    n =400;
    while ( n-- )
    {
        asm("nop"); asm("nop"); asm("nop"); asm("nop");
        asm("nop"); asm("nop"); asm("nop"); asm("nop");
        asm("nop"); asm("nop");

    }
    write_reg(CORE_CTRL_BASE_ADDR, 0X01);
}

/**
 * @brief  Start the function of RF, refer to MODULE_TEST for use.
 */
void rf_start()
{
    write_reg(BB_START_P_REG, 1);
}

void rf_start_in_odd_frequency_point(void)
{
    write_reg(0x40070080, 0x0);//bb crtl signal from software mode 
    write_reg(0x40020000, 0x27);//rf crtl signal from software mode 

    uint32_t reg_val =0;
    /*0x40070088 Bit[15:8] =150 ,rx rf wait time set to 150us*/
    reg_val =0;
    reg_val =read_reg(0x40070088);
    reg_val &= ~0xFF00;
    reg_val |= 150<<8;
    write_reg(0x40070088, reg_val);

    /**/
    reg_val =0;
    reg_val =read_reg(0x40070084);
    reg_val |=1<<31;//en_trx signal set 1 by software
    //reg_val &=~(1<<30);//normal
    reg_val |=(1<<30);////rxb_tx signal set 1 by software
    write_reg(0x40070084, reg_val);
    delay_us(100);

    write_reg(BB_START_P_REG, 1);

    delay_us(100);
    reg_val =0;
    reg_val =read_reg(0x40070084);

    reg_val |=1<<28;//set start_adc set 1 by software
    write_reg(0x40070084, reg_val);

}
void rf_set_recv_in_odd_frequency_point(void)
{
    uint32_t reg_val =0;
    
    reg_val =read_reg(0x40070084);
    reg_val &=~(1<<28);
    write_reg(0x40070084, reg_val);
    delay_us(100);
    reg_val =0;
    reg_val =read_reg(0x40070084);
    reg_val &=~(1<<31);
    reg_val |=(1<<30);

    write_reg(0x40070084, reg_val);
}
void rf_set_odd_to_normal(void)
{
    uint32_t reg_val =0;
    
    reg_val =read_reg(0x40070084);
    reg_val &=~(1<<28);
    write_reg(0x40070084, reg_val);
    delay_us(100);
    reg_val =0;
    reg_val =read_reg(0x40070084);
    reg_val &=~(1<<31);
    reg_val &=~(1<<30);
    write_reg(0x40070084, reg_val);


    write_reg(0x40070080, 0x1);//bb crtl signal from software mode 
    write_reg(0x40020000, 0x1);//rf crtl signal from software mode 
}

uint8_t rf_frequency_corresponds_to_channel(uint16_t freq)
{
    uint8_t ret =0;
    uint16_t tmp_frq = 0;
    tmp_frq = freq - 2402;

    if((tmp_frq > 80)&&(tmp_frq < 84))
    {
        return ret;
    }
    else if(tmp_frq >= 84)
    {
        tmp_frq -=4;
    }
    
    if(tmp_frq%2)
    {
        tmp_frq =tmp_frq - 1;
    }
    
    tmp_frq = tmp_frq/2;
    
    if(tmp_frq == 0x0)
    {
        ret = 37;
    }
    else if((tmp_frq>0)&&(tmp_frq<12))
    {
        ret = tmp_frq - 1;
    }
    else if(tmp_frq ==12)
    {
        ret =38;
    }
    else if((tmp_frq>12)&&(tmp_frq<39))
    {
        ret = tmp_frq - 2;
    }
    else if(tmp_frq ==39)
    {
        ret =39;
    }
    else 
    {
        ret = tmp_frq;
    }
    return ret;
}

/**
 * @brief  Get the offset of FIFO_RAM, if bbram config as FIFO function, refer to MODULE_TEST for use.
 * @return uint8_t                    The offset of BB_RAM.
 */
uint8_t rf_get_buffer_offset()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_BUFFER_STATUS_REG);
    reg_val = (reg_val & BB_BUFFER_POINTER_MASK) >> 2;

    return  (uint8_t)reg_val;
}

uint8_t rf_get_buffer_status()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_BUFFER_STATUS_REG);

    return  (uint8_t)reg_val;
}

/**
 * @brief  Get the status of RF interrupt.
 * @return uint8_t                    @ref int status "user guide: reg_interrupt_RO"
 */
uint8_t rf_get_int_status()
{
    return  (uint8_t)read_reg(BB_REG_INTERRUPT_READ_ONLY_REG);
}

void rf_enable_int( uint16_t int_en)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);

#if 1
    reg_val |= int_en;
#else
    switch( int_en )
    {
    case BB_INT_TX_EN_MASK:
        reg_val |= BB_INT_TX_EN_MASK;
        break;
    case BB_INT_RX_EN_MASK:
        reg_val |= BB_INT_RX_EN_MASK;
        break;
    case BB_INT_CRC_ERR_EN_MASK:
        reg_val |= BB_INT_CRC_ERR_EN_MASK;
        break;
    case BB_INT_LEN_ERR_EN_MASK:
        reg_val |= BB_INT_LEN_ERR_EN_MASK;
        break;
    case BB_INT_TYPE_ERR_EN_MASK:
        reg_val |= BB_INT_TYPE_ERR_EN_MASK;
        break;
    case BB_INT_SYNC_ERR_EN_MASK:
        reg_val |= BB_INT_SYNC_ERR_EN_MASK;
        break;

    default:
        reg_val = 0;
        break;
    }
#endif

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_disable_int( uint16_t int_dis)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);

#if 1
     reg_val &= ~int_dis;
#else
    switch( int_dis )
    {
    case BB_INT_TX_EN_MASK:
        reg_val &= ~BB_INT_TX_EN_MASK;
        break;
    case BB_INT_RX_EN_MASK:
        reg_val &= ~BB_INT_RX_EN_MASK;
        break;
    case BB_INT_CRC_ERR_EN_MASK:
        reg_val &= ~BB_INT_CRC_ERR_EN_MASK;
        break;
    case BB_INT_LEN_ERR_EN_MASK:
        reg_val &= ~BB_INT_LEN_ERR_EN_MASK;
        break;
    case BB_INT_TYPE_ERR_EN_MASK:
        reg_val &= ~BB_INT_TYPE_ERR_EN_MASK;
        break;
    case BB_INT_SYNC_ERR_EN_MASK:
        reg_val &= ~BB_INT_SYNC_ERR_EN_MASK;
        break;

    default:
        reg_val = 0;
        break;
    }
#endif

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_mask_int( uint16_t int_mask)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);

#if 1
	reg_val |= int_mask;
#else
    switch( int_mask )
    {
    case BB_INT_TX_MASK_MASK:
        reg_val |= BB_INT_TX_MASK_MASK;
        break;
    case BB_INT_RX_MASK_MASK:
        reg_val |= BB_INT_RX_MASK_MASK;
        break;
    case BB_INT_CRC_ERR_MASK_MASK:
        reg_val |= BB_INT_CRC_ERR_MASK_MASK;
        break;
    case BB_INT_LEN_ERR_MASK_MASK:
        reg_val |= BB_INT_LEN_ERR_MASK_MASK;
        break;
    case BB_INT_TYPE_ERR_MASK_MASK:
        reg_val |= BB_INT_TYPE_ERR_MASK_MASK;
        break;
    case BB_INT_SYNC_ERR_MASK_MASK:
        reg_val |= BB_INT_SYNC_ERR_MASK_MASK;
        break;

    default:
        reg_val = 0;
        break;
    }
#endif

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_unmask_int( uint16_t int_unmask)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);

#if 1
	reg_val &= ~int_unmask;
#else
    switch( int_unmask )
    {
    case BB_INT_TX_MASK_MASK:
        reg_val &= ~BB_INT_TX_MASK_MASK;
        break;
    case BB_INT_RX_MASK_MASK:
        reg_val &= ~BB_INT_RX_MASK_MASK;
        break;
    case BB_INT_CRC_ERR_MASK_MASK:
        reg_val &= ~BB_INT_CRC_ERR_MASK_MASK;
        break;
    case BB_INT_LEN_ERR_MASK_MASK:
        reg_val &= ~BB_INT_LEN_ERR_MASK_MASK;
        break;
    case BB_INT_TYPE_ERR_MASK_MASK:
        reg_val &= ~BB_INT_TYPE_ERR_MASK_MASK;
        break;
    case BB_INT_SYNC_ERR_MASK_MASK:
        reg_val &= ~BB_INT_SYNC_ERR_MASK_MASK;
        break;

    default:
        reg_val = 0;
        break;
    }
#endif

    write_reg(BB_INT_CTRL_REG, reg_val);
}

/**
 * @brief  Cler the interrupt status of RF.
 * @param  uint32_t                    @ref interrupt bit "user guide: intp_cls"
 */
void rf_int_clear( uint32_t int_clear )
{
    write_reg( BB_STATUS_CLEAR_REG, int_clear );
}

/**
 * @brief  Breif
 * @param  buffer_index               0 if single ram, cooperate rf_get_buffer_offset() if FIFO mode.
 * @return int8_t                     the RSSI of received packets.
 */
int8_t  rf_get_rssi( uint8_t buffer_index )
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_REG_INTERRUPT_READ_ONLY_REG);
    if ( 0 == buffer_index )
    {
        reg_val = ( reg_val & BB_BUF0_RSSI_MASK ) >> BB_BUF0_RSSI_SHIFT;
    }
    else
    {
        reg_val = ( reg_val & BB_BUF1_RSSI_MASK ) >> BB_BUF1_RSSI_SHIFT;
    }

    return (uint8_t)reg_val;
}

void rf_set_rx_win_size(uint16_t win_size)
{
    uint32_t    reg_val = 0;

    reg_val = read_reg(BB_REG_BASEBAND_STATE_RW1_REG);
    reg_val &= ~BB_CS_RXWINSZ_MASK;
    reg_val |= win_size << BB_CS_RXWINSZ_SHIFT;

    reg_val &= ~BB_CS_RXWIDE_MASK;

    write_reg(BB_REG_BASEBAND_STATE_RW1_REG, reg_val);
}

inline void rf_set_bb_mode( rf_bb_mode_t rf_bb_mode)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(TOP_MODULE_MODE_REG);
    reg_val &= ~TOP_RF_BB_MODE_MASK;
    reg_val |= rf_bb_mode << TOP_RF_BB_MODE_SHIFT;

    if(chip_verson_check())
    {
        if(!(reg_val & TOP_WATCH_DOG_ON_MASK))
        {
            reg_val |= TOP_WATCH_DOG_ON_MASK;
        }
        else
        {
            reg_val &= ~TOP_WATCH_DOG_ON_MASK;
        }

    }

    write_reg(TOP_MODULE_MODE_REG, reg_val);
};
#if 0
void rf_bb_mode_config()
{
    sys_set_port_mux(PAD_GPIO_00, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_01, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_02, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_03, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_04, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_05, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_06, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_07, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_08, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_09, PAD_MUX_FUNCTION_2 | 3);

    sys_set_port_mux(PAD_GPIO_10, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_11, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_12, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_13, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_14, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_15, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_16, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_17, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_18, PAD_MUX_FUNCTION_2 | 3);
    sys_set_port_mux(PAD_GPIO_19, PAD_MUX_FUNCTION_2 | 3);

    rf_set_bb_mode( EXT_RF_CHIP_BB );
}
#endif
/*
void rf_set_len_mode(bb_pack_len_mode_t      len_mode )
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_REG_BASEBAND_STATE_RW5_REG);
    reg_val &= ~BB_RX_LENGTH_MODE_MASK;
    reg_val |= ((uint8_t)len_mode) << BB_RX_LENGTH_MODE_SHIFT;
}
*/

/**
 * @brief  Get the length of Rx PDU, only support BLE packets.
 * @param  buffer_index               0 if single ram, cooperate rf_get_buffer_offset() if FIFO mode.
 * @return uint8_t                    the length of PDU.
 */
uint8_t rf_get_rx_payload_len(uint8_t buffer_index)
{
    uint32_t reg_val = 0;
#if 1

    reg_val = read_reg(BB_REG_BASEBAND_STATE_OUTPUT_REG);
    reg_val = (reg_val & BB_PKT_LENGTH_MASK) >> BB_PKT_LENGTH_SHIFT;
    return (uint8_t)reg_val;

#else
    reg_val = read_reg(BB_REG_BASEBAND_STATE_RW5_REG);
    if ( 0 == buffer_index )
    {
        reg_val = ( reg_val & BB_RXLEN_CPU0_MASK ) >> BB_RXLEN_CPU0_SHIFT;
    }
    else
    {
        reg_val = ( reg_val & BB_RXLEN_CPU1_MASK ) >> BB_RXLEN_CPU1_SHIFT;
    }

    return reg_val;
#endif
}

/**
 * @brief  Breif
 * @param  buffer_index               0 if single ram, cooperate rf_get_buffer_offset() if FIFO mode.
 * @return uint8_t                    the channel_idx of received packets, correspond to the channel of rf_set_rx_sync_words.
 */
uint8_t rf_get_data_from_channel( uint8_t buffer_index )
{
    uint32_t reg_val = 0;

    reg_val = read_reg( BB_REG_INTERRUPT_READ_ONLY_REG );

    if ( 0 == buffer_index )
    {
        reg_val &= BB_BUF0_ACSYNC_FOUND_ADDR_NUM_MASK;
        reg_val = reg_val >> BB_BUF0_ACSYNC_FOUND_ADDR_NUM_SHIFT;
    }
    else
    {
        reg_val &= BB_BUF1_ACSYNC_FOUND_ADDR_NUM_MASK;
        reg_val = reg_val >> BB_BUF1_ACSYNC_FOUND_ADDR_NUM_SHIFT;
    }

    return reg_val;
}

/**
 * @brief  Enable/Disable CRC and Whitening.
 * @param  whiting_ctrl               ON/OFF.
 * @param  crc_ctrl                   ON/OFF.
 */
void rf_set_whit_and_crc(bool whiting_ctrl, bool crc_ctrl)
{
    uint32_t reg_val = 0;

    reg_val = read_reg( BB_REG_BIT_STREAM_REG );
    reg_val &= ~(BB_WHIT_DSB_MASK | BB_CRC_DSB_MASK);
    reg_val |= ( (~whiting_ctrl) & 1 ) << BB_WHIT_DSB_SHIFT;
    reg_val |= ( (~crc_ctrl) & 1 ) << BB_CRC_DSB_SHIFT;

    write_reg( BB_REG_BIT_STREAM_REG, reg_val);
}

/**
 * @brief  The length of Tx packets, PDU length if send BLE packets.
 * @param  tx_len                     the setting of length.
 */
void rf_set_tx_len( uint8_t tx_len )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( BB_REG_TYPE_LENGTH_REG );
    reg_val &= ~BB_TXADVLEN_MASK;
    reg_val |= (tx_len - 2 );

    reg_val |= 1 << BB_TX_DATA_INVERT_SHIFT;
    write_reg( BB_REG_TYPE_LENGTH_REG, reg_val );
}

/**
 * @brief  Config Tx premble.
 * @param  pre_mode                   @ref bb_tx_pre_mode_t "premble mode"
 * @param  pre                        the value of premble.
 */
void rf_set_tx_preamble( bb_tx_pre_mode_t pre_mode, uint32_t pre )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg(BB_REG_BASEBAND_STATE_RW2_REG);

    reg_val &= ~BB_CS_TXPREAMBLE_MODE_MASK;
    reg_val |= pre_mode << BB_CS_TXPREAMBLE_MODE_SHIFT;

    reg_val &= ~BB_CS_PREAMBLE_MASK;
    reg_val |= pre << BB_CS_PREAMBLE_SHIFT;

    write_reg(BB_REG_BASEBAND_STATE_RW2_REG, reg_val);
}

/**
 * @brief  Config Tx access address, send by LSB of [aa_high+aa_low] 
 * @param  aa_mode                    @ref bb_tx_aa_mode_t "access address mode"
 * @param  aa_high                    High-byte of access address. 0 if send BLE packets.
 * @param  aa_low                     Low-byte of access address.
 */
void rf_set_tx_aa( bb_tx_aa_mode_t aa_mode, uint8_t aa_high, uint32_t aa_low )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg(BB_REG_BASEBAND_STATE_RW2_REG);
    reg_val &= ~BB_CS_TXSYNCWORD_MODE_MASK;
    reg_val |= aa_mode << BB_CS_TXSYNCWORD_MODE_SHIFT;
    write_reg( BB_REG_BASEBAND_STATE_RW2_REG, reg_val );

    reg_val = read_reg(BB_AC_CODE_HIGHPART_REG);
    reg_val &= ~BB_CS_SYNCWORD1_HIGHPART_MASK;
    reg_val|= aa_high << BB_CS_SYNCWORD1_HIGHPART_SHIFT;
    write_reg(BB_AC_CODE_HIGHPART_REG, reg_val);

    write_reg(BB_AC_CODE1_REG, aa_low);
}

/**
 * @brief  Config RF power level
 * @param  rf_power                   0~7
 */
void rf_set_power( uint8_t rf_power )
{
    uint32_t    reg_val = 0;

    reg_val = read_reg( RFE_REG_MH );
    reg_val &= ~RFE_RF_POWER_L_MASK;
    reg_val |= (rf_power & 0x03 ) << 6;
    write_reg(RFE_REG_MH, reg_val);
    
    reg_val = read_reg( RFE_REG_HH );
    reg_val &= ~RFE_RF_POWER_H_MASK;
    reg_val |= (rf_power & 0x04) >> 2;
    write_reg(RFE_REG_HH, reg_val);
}

void rf_set_tx_pattern()
{
    uint32_t reg_val = 0;

    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW3_REG );
    reg_val &= ~BB_CS_FORMAT_MASK;
    reg_val |= 0x1C;
    write_reg( BB_REG_BASEBAND_STATE_RW3_REG, reg_val);

    reg_val = read_reg( BB_REG_BASEBAND_STATE_RW5_REG );
    reg_val &= ~BB_TX_TEST_MODE_TYPE_MASK;
    reg_val |= 0x0;
    write_reg( BB_REG_BASEBAND_STATE_RW5_REG, reg_val );
}

/*

void rf_set_tx_pre_and_sync()
{
    uint32_t    reg_val = 0;

    reg_val |= BB_TX_PRE << BB_CS_PREAMBLE_SHIFT;
    reg_val |= BB_TX_AA_MODE << BB_CS_TXSYNCWORD_MODE_SHIFT;
    reg_val |= BB_TX_PRE_MODE << BB_CS_TXPREAMBLE_MODE_SHIFT;
    write_reg(BB_REG_BASEBAND_STATE_RW2_REG, reg_val);

    write_reg(BB_AC_CODE1_REG, BB_TX_AA_LOW_4_BYTE);
}


void rf_set_int(rf_int_config_t config)
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);

    switch( config )
    {
        case RF_TX_INT_ENABLE:  reg_val |= BB_INT_TX_EN_MASK;       break;
        case RF_TX_INT_DISABLE: reg_val &= ~BB_INT_TX_EN_MASK;      break;
        case RF_TX_INT_MASK:    reg_val |= BB_INT_TX_MASK_MASK;     break;
        case RF_TX_INT_UNMASK:  reg_val &= ~BB_INT_TX_MASK_MASK;    break;
        case RF_TX_INT_CLEAR:   reg_val |= BB_INT_TX_CLEAR_MASK;    break;

        case RF_RX_INT_ENABLE:  reg_val |= BB_INT_RX_EN_MASK;       break;
        case RF_RX_INT_DISABLE: reg_val &= ~BB_INT_RX_EN_MASK;      break;
        case RF_RX_INT_MASK:    reg_val |= BB_INT_RX_MASK_MASK;     break;
        case RF_RX_INT_UNMASK:  reg_val &= ~BB_INT_RX_MASK_MASK;    break;
        case RF_RX_INT_CLEAR:   reg_val |= BB_INT_RX_CLEAR_MASK;    break;

        default: reg_val = 0; break;
    }

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_tx_int_enble()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x01;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_tx_int_disable()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val &= ~0x01;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_tx_int_mask()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x02;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_tx_int_unmask()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val &= ~0x02;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_tx_int_clear()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x04;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_rx_int_enble()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x08;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_rx_int_disable()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val &= ~0x08;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_rx_int_mask()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x10;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_rx_int_unmask()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val &= ~0x10;

    write_reg(BB_INT_CTRL_REG, reg_val);
}

void rf_rx_int_clear()
{
    uint32_t reg_val = 0;

    reg_val = read_reg(BB_INT_CTRL_REG);
    reg_val |= 0x20;

    write_reg(BB_INT_CTRL_REG, reg_val);
}
*/


#ifndef MODULE_TEST
ATTRIBUTE_ISR ATTRIBUTE_WEAK void rx_isr()
{
	//DO SOMETHING
}
#endif  //#ifndef MODULE_TEST

ATTRIBUTE_ISR ATTRIBUTE_WEAK void tx_isr()
{
	//DO SOMETHING
}

