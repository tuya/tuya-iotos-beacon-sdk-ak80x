/* ---------------------------------------------------------------
** File name:		rf.h
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

#ifndef     __RF_H_
#define     __RF_H_


/* Includes -----------------------------------------------------*/
#include "sys.h"

/* Macro --------------------------------------------------------*/

#define RX_CHANNEL_SINGLE				    (0x01)  //only enable single rx channel,default use channel 0
#define RX_CHANNEL_BOTH					    (0x02)  //enable both channel,use channel 0 and channel 1
#define ENABLE_RX_CHANNEL_NUM			    RX_CHANNEL_SINGLE

#define EORR_MODE_ACTIVE_BITS               (BB_RX_SW_MODE_36_BIT)

#define BB_SW_HIGH_1_BYTE                   (0x8E)
#define BB_SW_LOW_4_BYTE                    (0x89BED6AA)

#define BB_TX_PRE_MODE                      BB_TX_PRE_MODE_1_BYTE
#define BB_TX_AA_MODE                       BB_TX_AA_MODE_4_BYTE
#define BB_TX_PRE                           (0xAA)
#define BB_TX_AA_LOW_4_BYTE                 (0x8E89BED6)
#define BB_TX_AA_HIGH_1_BYTE                (0x00)

#define BB_CRC_RX_INT_COMBINED_CFG          (BB_CRC_WITHOUT_RX)     // BB_CRC_WITH_RX: CRC, RX interrupt raise together.
                                                                    // BB_CRC_WITHOUT_RX: CRC int raise without RX int.

#define BB_CRC_INTI_VAL                     (0x555555)
#define BB_WHIT_CTRL                        BB_WHIT_EN
#define BB_CRC_CTRL                         BB_CRC_EN
#define BB_WHIT_DSB                         (0x2000000)             // Disable whitening.
#define BB_WHIT_EN                          (0)                     // Enable whitening.
#define BB_CRC_DSB                          (0x1000000)             // Disable CRC24
#define BB_CRC_EN                           (0)                     // Enable CRC24

#define BB_RF_GFO_COMP                      (RF_GFO_DMX_COMP_EN | RF_GFO_DCC_COMP_EN)

#define BB_CHANNEL_0_PKG_LEN_MODE           BB_PACK_LEN_AUTO        // RX payload length got from package.
#define BB_CHANNEL_1_PKG_LEN_MODE           BB_PACK_LEN_AUTO        // RX payload length got from package.
#define BB_CHANNEL_0_PKG_LEN                (31)
#define BB_CHANNEL_1_PKG_LEN                (37)

#define BB_CHANNEL_NUMB                     (37)

#define BB_MEMEORY_ADDR                     (0x50000000)
#define BB_MEMEORY_SIZE                     (128)                   // 128 Bytes


/*****************************************************************
 * @registers
 */

#define BB_BASE_ADDR                        (0x40070000)

#define BB_SOFT_RESET_REG                   (BB_BASE_ADDR + 0x00)

#define BB_AGC_CONFIG_1_REG                 (BB_BASE_ADDR + 0x08)
#define BB_TARGET_SIG_PWR_MASK              (0x7C000000)
#define BB_TARGET_SIG_PWR_SHIFT             (26)

#define BB_PE_CTRL_REG                      (BB_BASE_ADDR + 0x18)
#define BB_POWER_SQR_THR_MASK               (0x3FF)
#define BB_POWER_SQR_THR_SHIFT              (0)


/*
 * reg_rxgfsk_pe
 * @ 0x1C
 */
#define BB_REG_RXGFSK_PE_REG                (BB_BASE_ADDR + 0x1C)
#define BB_PE_PWR_SQR_UN_THR_MASK           (0x3FF)
#define BB_PE_PWR_SQR_UN_THR_SHIFT          (0)

#define BB_RX_P2P_THR_REG                   (BB_BASE_ADDR + 0x20)
#define BB_TEMP_MASK                        (0x1FF80000)
#define BB_TEMP_SHIFT                       (19)
#define BB_RX_1ST_P2P_MASK                  (0x7FF00)
#define BB_RX_1ST_P2P_SHIFT                 (8)

#define BB_GFO_COMPENSATE_REG               (BB_BASE_ADDR + 0x30)

#define BB_RX_GFSK_DCC_DL_MODE_REG          (BB_BASE_ADDR + 0x38)
#define BB_AC_CODE_SEL_REG                  (BB_BASE_ADDR + 0x3C)   // 0: only channel0 work. 1: 2 channels all work.
#define BB_AC_CODE1_REG                     (BB_BASE_ADDR + 0x40)
#define BB_AC_CODE2_REG                     (BB_BASE_ADDR + 0x44)

/*
 * ac_code_highpart
 * @ 0x34
 */
#define BB_AC_CODE_HIGHPART_REG             (BB_BASE_ADDR + 0x48)
#define BB_CS_SYNCWORD2_HIGHPART_MASK       (0xFF00)
#define BB_CS_SYNCWORD2_HIGHPART_SHIFT      (8)
#define BB_CS_SYNCWORD1_HIGHPART_MASK       (0xFF)
#define BB_CS_SYNCWORD1_HIGHPART_SHIFT      (0)


/*
 * reg_bit_stream
 * @ 0x40
 */
#define BB_REG_CORR_MODE_REG                (BB_BASE_ADDR + 0x4C)   // 0: 32Bits; 1: 36bits; 2: 40Bits

/*
 * reg_ac_correlator
 * @ 0x50
 */
#define BB_REG_AC_CORRELATOR_REG            (BB_BASE_ADDR + 0x50)
#define BB_SYNC_TUNE_MASK                   (0x7800)
#define BB_SYNC_TUNE_SHIFT                  (11)
#define BB_CORR_WINDOW_MASK_MASK            (0x10)
#define BB_CORR_WINDOW_MASK_SHIFT           (4)
#define BB_DPCORR_EN_MASK                   (0x8)
#define BB_DPCORR_EN_SHIFT                  (3)
#define BB_SYNCERR_THR_MASK                 (0x7)
#define BB_SYNCERR_THR_SHIFT                (0)

/*
 * reg_bit_stream
 * @ 0x40
 */
#define BB_REG_BIT_STREAM_REG               (BB_BASE_ADDR + 0x5C)
#define BB_WHIT_DSB_MASK                    (0x2000000)
#define BB_WHIT_DSB_SHIFT                   (25)
#define BB_CRC_DSB_MASK                     (0x1000000)
#define BB_CRC_DSB_SHIFT                    (24)
#define BB_CS_CRCINIT_MASK                  (0xFFFFFF)
#define BB_CS_CRCINIT_SHIFT                 (0)

#define BB_REG_TYPE_LENGTH_REG              (BB_BASE_ADDR + 0x60)
#define BB_TX_DATA_INVERT_MASK              (0x2000)
#define BB_TX_DATA_INVERT_SHIFT             (13)
#define BB_TXTYPE_MASK                      (0x1E00)
#define BB_TXTYPE_SHIFT                     (9)
#define BB_TXRXADD_MASK                     (0x100)
#define BB_TXRXADD_SHIFT                    (8)
#define BB_TXTXADD_MASK                     (0x80)
#define BB_TXTXADD_SHIFT                    (7)
#define BB_TXADVLEN_MASK                    (0x7F)
#define BB_TXADVLEN_SHIFT                   (0)


#define BB_RX_TX_SEL_REG                    (BB_BASE_ADDR + 0x64)
#define BB_CHANNEL_NUM_REG                  (BB_BASE_ADDR + 0x68)

/*
 * reg_baseband_state_rw1
 * @ 0x50
 */
#define BB_REG_BASEBAND_STATE_RW1_REG       (BB_BASE_ADDR + 0x6C)
#define BB_CRCERR_MASK_MASK                 (0x10000000)
#define BB_CRCERR_MASK_SHIFT                (28)
#define BB_RF_PLL_SETTLING_TIME_MASK        (0xFF00000)
#define BB_RF_PLL_SETTLING_TIME_SHIFT       (20)
#define BB_PKTCNTL_USECSWINSZ_MASK          (0x80000)
#define BB_PKTCNTL_USECSWINSZ_SHIFT         (19)
#define BB_CS_RXWIDE_MASK                   (0x40000)
#define BB_CS_RXWIDE_SHIFT                  (18)
#define BB_RXWINSZDEF_MASK                  (0x3C000)
#define BB_RXWINSZDEF_SHIFT                 (14)
#define BB_CS_RXWINSZ_MASK                  (0x3FFF)
#define BB_CS_RXWINSZ_SHIFT                 (0)

#define BB_START_P_REG                      (BB_BASE_ADDR + 0x70)

/*
 * reg_baseband_state_rw2
 * @ 0x5C
 */
#define BB_REG_BASEBAND_STATE_RW2_REG       (BB_BASE_ADDR + 0x74)
#define BB_CS_TXPREAMBLE_MODE_MASK          (0xC000000)
#define BB_CS_TXPREAMBLE_MODE_SHIFT         (26)
#define BB_CS_TXSYNCWORD_MODE_MASK          (0x3000000)
#define BB_CS_TXSYNCWORD_MODE_SHIFT         (24)
#define BB_CS_PREAMBLE_MASK                 (0xFFFFFF)
#define BB_CS_PREAMBLE_SHIFT                (0)

/*
 * reg_baseband_state_rw3
 * @ 0x78
 */
#define BB_REG_BASEBAND_STATE_RW3_REG       (BB_BASE_ADDR + 0x78)
#define BB_CS_FORMAT_MASK                   (0x1F)
#define BB_CS_FORMAT_SHIFT                  (0)

/*
 * reg_baseband_state_rw5
 * @ 0x64
 */
#define BB_REG_BASEBAND_STATE_RW5_REG       (BB_BASE_ADDR + 0x84)
#define BB_SOFT_EN_TRX_MASK                 (0x80000000)
#define BB_SOFT_EN_TRX_SHIFT                (31)
#define BB_SOFT_RXB_TX_MASK                 (0x40000000)
#define BB_SOFT_RXB_TX_SHIFT                (30)
#define BB_SOFT_EN_PA_MASK                  (0x20000000)
#define BB_SOFT_EN_PA_SHIFT                 (29)
#define BB_SOFT_START_RX_ADC_MASK           (0x10000000)
#define BB_SOFT_START_RX_ADC_SHIFT          (28)
#define BB_TXPWRDN_MASK                     (0xFF00000)
#define BB_TXPWRDN_SHIFT                    (20)
#define BB_TX_TEST_MODE_TYPE_MASK           (0xF0000)
#define BB_TX_TEST_MODE_TYPE_SHIFT          (16)
#define BB_RXLEN_CPU1_MASK                  (0xFE00)
#define BB_RXLEN_CPU1_SHIFT                 (9)
#define BB_RXLEN_CPU0_MASK                  (0x1FC)
#define BB_RXLEN_CPU0_SHIFT                 (2)
#define BB_RX_LENGTH_MODE_MASK              (0x3)
#define BB_RX_LENGTH_MODE_SHIFT             (0)

#define BB_RX_CHNL1_LENGTH_MODE_MASK        (0x2)
#define BB_RX_CHNL1_LENGTH_MODE_SHIFT       (1)
#define BB_RX_CHNL0_LENGTH_MODE_MASK        (0x1)
#define BB_RX_CHNL0_LENGTH_MODE_SHIFT       (0)


/*
 * reg_baseband_state_output
 * @ 0x90
 */
#define BB_REG_BASEBAND_STATE_OUTPUT_REG    (BB_BASE_ADDR + 0x90)
#define BB_RSSI_MASK                        (0x3FC0000)
#define BB_RSSI_SHIFT                       (18)
#define BB_SYNCERR_MASK                     (0x20000)
#define BB_SYNCERR_SHIFT                    (17)
#define BB_TYPEERR_MASK                     (0x10000)
#define BB_TYPEERR_SHIFT                    (16)
#define BB_LENERR_MASK                      (0x8000)
#define BB_LENERR_SHIFT                     (15)
#define BB_CRCERR_MASK                      (0x4000)
#define BB_CRCERR_SHIFT                     (14)
#define BB_PKT_LENGTH_MASK                  (0x3FC0)
#define BB_PKT_LENGTH_SHIFT                 (6)
#define BB_RFE_GAIN_MASK                    (0x38)
#define BB_RFE_GAIN_SHIFT                   (3)
#define BB_PGA_GAIN_MASK                    (0x7)
#define BB_PGA_GAIN_SHIFT                   (0)


#define BB_BUFFER_CTRL_REG                  (BB_BASE_ADDR + 0x94)
#define BB_BUFFER_WRITE_FINISH_MASK         (0x01)
#define BB_BUFFER_READ_FINISH_MASK          (0x02)


#define BB_INT_CTRL_REG                     (BB_BASE_ADDR + 0x98)
#define BB_INT_TX_EN_MASK                   (0x0001)
#define BB_INT_TX_MASK_MASK                 (0x0002)
#define BB_INT_RX_EN_MASK                   (0x0004)
#define BB_INT_RX_MASK_MASK                 (0x0008)
#define BB_INT_CRC_ERR_EN_MASK              (0x0010)
#define BB_INT_CRC_ERR_MASK_MASK            (0x0020)
#define BB_INT_LEN_ERR_EN_MASK              (0x0040)
#define BB_INT_LEN_ERR_MASK_MASK            (0x0080)
#define BB_INT_TYPE_ERR_EN_MASK             (0x0100)
#define BB_INT_TYPE_ERR_MASK_MASK           (0x0200)
#define BB_INT_SYNC_ERR_EN_MASK             (0x0400)
#define BB_INT_SYNC_ERR_MASK_MASK           (0x0800)


/*
 * reg interrupt read only
 * @ 0x7C
 */
#define BB_REG_INTERRUPT_READ_ONLY_REG      (BB_BASE_ADDR + 0x9C)
#define BB_BUF1_RSSI_MASK                   (0xFF00000)
#define BB_BUF1_RSSI_SHIFT                  (20)
#define BB_BUF1_ACSYNC_FOUND_ADDR_NUM_MASK  (0x80000)
#define BB_BUF1_ACSYNC_FOUND_ADDR_NUM_SHIFT (19)
#define BB_BUF0_RSSI_MASK                   (0x7F800)
#define BB_BUF0_RSSI_SHIFT                  (11)
#define BB_BUF0_ACSYNC_FOUND_ADDR_NUM_MASK  (0x400)
#define BB_BUF0_ACSYNC_FOUND_ADDR_NUM_SHIFT (10)
#define BB_BUF1_SYNCERR_RAW_INT_STATE_MASK  (0x200)
#define BB_BUF1_SYNCERR_RAW_INT_STATE_SHIFT (9)
#define BB_BUF1_TYPEERR_RAW_INT_STATE_MASK  (0x100)
#define BB_BUF1_TYPEERR_RAW_INT_STATE_SHIFT (8)
#define BB_BUF1_LENERR_RAW_INT_STATE_MASK   (0x80)
#define BB_BUF1_LENERR_RAW_INT_STATE_SHIFT  (7)
#define BB_BUF1_CRCERR_RAW_INT_STATE_MASK   (0x40)
#define BB_BUF1_CRCERR_RAW_INT_STATE_SHIFT  (6)
#define BB_BUF0_SYNCERR_RAW_INT_STATE_MASK  (0x20)
#define BB_BUF0_SYNCERR_RAW_INT_STATE_SHIFT (5)
#define BB_BUF0_TYPEERR_RAW_INT_STATE_MASK  (0x10)
#define BB_BUF0_TYPEERR_RAW_INT_STATE_SHIFT (4)
#define BB_BUF0_LENERR_RAW_INT_STATE_MASK   (0x8)
#define BB_BUF0_LENERR_RAW_INT_STATE_SHIFT  (3)
#define BB_BUF0_CRCERR_RAW_INT_STATE_MASK   (0x4)
#define BB_BUF0_CRCERR_RAW_INT_STATE_SHIFT  (2)
#define BB_CTRL2REG_RX_RAW_INT_STATE_MASK   (0x2)
#define BB_CTRL2REG_RX_RAW_INT_STATE_SHIFT  (1)
#define BB_CTRL2REG_TX_RAW_INT_STATE_MASK   (0x1)
#define BB_CTRL2REG_TX_RAW_INT_STATE_SHIFT  (0)
#define BB_ALL_INT_MASK                     (0x3FF)


#define BB_RAM_MODEL_REG                    (BB_BASE_ADDR + 0xA0)

#define BB_BUFFER_STATUS_REG                (BB_BASE_ADDR + 0xA4)
#define BB_BUFFER_FULL_MASK                 (0x0001)
#define BB_BUFFER_EMPTY_MASK                (0x0002)
#define BB_BUFFER_POINTER_MASK              (0x01FC)

#define BB_STATUS_CLEAR_REG                 (BB_BASE_ADDR + 0xAC)
#define BB_TX_INT_CLEAR_MASK                (0x01)
#define BB_RX_INT_CLEAR_MASK                (0x02)
#define BB_BUF0_CRC_ERR_INT_CLEAR_MASK      (0x04)
#define BB_BUF0_LEN_ERR_INT_CLEAR_MASK      (0x08)
#define BB_BUF0_TYPE_ERR_INT_CLEAR_MASK     (0x10)
#define BB_BUF0_SYNC_ERR_INT_CLEAR_MASK     (0x20)
#define BB_BUF0_INT_CLEAR_ALL_MASK          (0x3E)	// buffer0 + rx

#define BB_BUF1_CRC_ERR_INT_CLEAR_MASK      (0x40)
#define BB_BUF1_LEN_ERR_INT_CLEAR_MASK      (0x80)
#define BB_BUF1_TYPE_ERR_INT_CLEAR_MASK     (0x100)
#define BB_BUF1_SYNC_ERR_INT_CLEAR_MASK     (0x200)
#define BB_BUF1_INT_CLEAR_ALL_MASK          (0x3C2)	// buffer1 + rx

#define BB_INT_CLEAR_ALL_MASK               (0x3FF)


#define BB_DFE_FFF_COEF_REG                 (BB_BASE_ADDR + 0x54)
#define BB_DFE_FBF_COEF_REG                 (BB_BASE_ADDR + 0x58)
#define BB_LEN_ERR_LIMIT_REG                (BB_BASE_ADDR + 0x7C)


/* Structures ---------------------------------------------*/

typedef enum bb_crc_rx_int_e
{
    BB_CRC_WITH_RX          = 0,
    BB_CRC_WITHOUT_RX       = 1
}bb_crc_rx_int_t;

typedef enum bb_tx_pre_mode_s
{
    BB_TX_PRE_MODE_1_BYTE   = 0,
    BB_TX_PRE_MODE_2_BYTE   = 1,
    BB_TX_PRE_MODE_3_BYTE   = 2
}bb_tx_pre_mode_t;

typedef enum bb_tx_aa_mode_s
{
    BB_TX_AA_MODE_3_BYTE    = 0,
    BB_TX_AA_MODE_4_BYTE    = 1,
    BB_TX_AA_MODE_5_BYTE    = 2
}bb_tx_aa_mode_t;

typedef enum bb_pack_len_mode_s
{
    BB_PACK_LEN_AUTO        = 0,
    BB_PACK_LEN_MANUAL      = 1
}bb_pack_len_mode_t;

enum bb_channel_sel
{
    BB_CHANNEL_0_ONLY       = 0,
    BB_CHANNEL_0_1          = 1
};

typedef enum bb_ram_model
{
    BBRAM_MODEL_FIFO        = 0,
    BBRAM_MOBEL_SINGLE_RAM  = 2
} bb_ram_model_t;

typedef enum bb_channel
{
    BB_CHANNEL_0            = 1,
    BB_CHANNEL_1            = 2
} bb_channel_t;

typedef enum bb_rx_syncword_mode_e
{
    BB_RX_SW_MODE_32_BIT    = 0,
    BB_RX_SW_MODE_36_BIT    = 1,
    BB_RX_SW_MODE_40_BIT    = 2
}bb_rx_syncword_mode_t;

typedef enum bbram_mode
{
    BBRAM_FIFO_RX           = 0,
    BBRAM_FIFO_TX           = 1,
    BBRAM_SINGLE_RAM_RX     = 2,
    BBRAM_SINGLE_RAM_TX     = 3,
    BBRAM_FIFO_TXRX         = 4     // RX: use buffer0 (0-63). TX: use buffer1 (64_127).
} bbram_mode_t;

typedef enum rx_tx_mode
{
    RF_TX_MODE              = 0,
    RF_RX_MODE              = 1
} rx_tx_mode_t;

typedef enum rf_gfo_comp
{
    RF_GFO_COMP_DISABLE     = 0,
    RF_GFO_DMX_COMP_EN      = 1,
    RF_GFO_DCC_COMP_EN      = 2
} rf_gfo_comp_t;

/* Global Variables ---------------------------------------------*/

/* Extern -------------------------------------------------------*/

/* Function define ----------------------------------------------*/

#define rf_config_start()               write_reg(BB_SOFT_RESET_REG, 1)
#define rf_config_end()                 write_reg(BB_SOFT_RESET_REG, 0)
#define rf_soft_reset()                 write_reg(BB_SOFT_RESET_REG, 1)
#define rf_read_finish()                write_reg(BB_BUFFER_CTRL_REG, BB_BUFFER_READ_FINISH_MASK)
#define rf_write_finish()               write_reg(BB_BUFFER_CTRL_REG, BB_BUFFER_WRITE_FINISH_MASK)
#define rf_set_channel_num(a)           write_reg(BB_CHANNEL_NUM_REG, a)
#define rf_set_rx_channels(a)           write_reg(BB_AC_CODE_SEL_REG, a)
#define rf_set_rx_phase_intert(a)       write_reg(TOP_PHASE_INVERT_REG, a)
#define rf_get_int()                    ( BB_ALL_INT_MASK & read_reg( BB_REG_INTERRUPT_READ_ONLY_REG ) )

/* Function prototype -------------------------------------------*/


void rf_set_rx_sync_words( uint8_t channel, uint8_t sw_high, uint32_t sw_low);
void rf_set_bbram_mode( bbram_mode_t bbram_model );
void rf_set_rx_tx_mode( rx_tx_mode_t rx_tx_model );
void rf_init(void);
void rf_start(void);
uint8_t rf_get_buffer_offset(void);
uint8_t rf_get_buffer_status(void);
uint8_t rf_get_int_status(void);
void rf_set_compensate( rf_gfo_comp_t gfo_comp );
//void rf_set_int(rf_int_config_t config);
void rf_enable_int( uint16_t int_en);
void rf_disable_int( uint16_t int_dis);
void rf_mask_int( uint16_t int_mask);
void rf_unmask_int( uint16_t int_unmask);
void rf_int_clear( uint32_t int_clear );
int8_t  rf_get_rssi( uint8_t buffer_index );
void rf_set_rx_sync_bits(uint8_t sync_bits_mode);
void rf_set_rx_win_size(uint16_t win_size);
void rf_set_bb_mode( rf_bb_mode_t rf_bb_mode);
void rf_set_rx_len( bb_channel_t channel_index, bb_pack_len_mode_t len_mode, uint8_t len );
void rf_set_tx_len( uint8_t tx_len );
void rf_set_tx_pre_and_sync(void);
void rf_set_tx_preamble( bb_tx_pre_mode_t pre_mode, uint32_t pre );
void rf_set_tx_aa( bb_tx_aa_mode_t aa_mode, uint8_t aa_high, uint32_t aa_low );
uint8_t rf_get_rx_payload_len(uint8_t buffer_index);
uint8_t rf_get_data_from_channel( uint8_t buffer_index );
void rf_set_whit_and_crc(bool whiting_ctrl, bool crc_ctrl);
void rf_set_crc_rx_int_mode( uint8_t on_off );
void rf_set_tx_pattern(void);

void rf_bb_mode_config(void);
void rf_set_power( uint8_t rf_power );



#endif  // __RF_H_

