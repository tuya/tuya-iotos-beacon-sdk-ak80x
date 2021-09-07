/* ---------------------------------------------------------------
** File name:		lvd.h
** Brief:
** COPYRIGHT(C) ZXW 2020
** ---------------------------------------------------------------
** Module Information:
**
** Revision History ----------------------------------------------
** ---------------------------------------------------------------
**  Date        Version     Action                  Author
** ---------------------------------------------------------------
**  2019-6-20   1.0         Created
**
** ---------------------------------------------------------------
*/

#ifndef   __LVD_H_
#define   __LVD_H_





#define LVD_ULL_STATE                       (0x40020074)
#define INTRRUPT_REG_L                      (0x40020060)
#define INTRRUPT_REG_H                      (0x40020064)

#define TOP_CTRL_REG                        (0x40120100)

#define PMU_REG_ML                          (0x40120120)//THR BIT[6:5]
#define LVD_THR_MASK                        0x60
#define LVD_THR_SHIFT                       5

#define MODULE_MODE_REG                     (0x4012018C)
#define WAKEUP_SOURE_STATE                  (0x40120190)


#define INT_TYPE_CFG_H                      (0x40020064)//1:level source   0:pulse
#define INT_TYPE_CFG_L                      (0x40020060)//1:level source   0:pulse

/*MODULE_MODE*/
#define LVD_EN                              (0x00000020)//BIT[5]
#define CPU_CLK_SEL_2M                      (0x00000000)//BIT[4:3]00:2M  
#define CPU_CLK_SEL_4M                      (0x00000008)//01:4M 
#define CPU_CLK_SEL_8M                      (0x00000010)//10:8M
#define CPU_CLK_SEL_16M                     (0x00000018)//11:16M
#define WDG_EN                              (0x00000004)//BIT[2]


/*TOP_CTRL_REG*/
#define LVD_PWR_CTRL_EN                     (0x02)//TOP_CTRL_REG   BIT[2]


/*WAKE UP SOUCE BIT*/
#define WAKEUP_BY_FPO						(0x00)//First Power On
#define WAKEUP_BY_LVD						(0x01)//LVD
#define WAKEUP_BY_WTD						(0x02)//Watch Dog
#define WAKEUP_BY_WUT						(0x04)//Wake Up Timer


#define VLD_LEVEL_IRQ                       (0x400)//BIT[10]

typedef enum lvd_threshold
{
    LVD_THR_0      = 0,//MAX //Typical:2.5V
    LVD_THR_1      = 1,      //Typical:2.3V
    LVD_THR_2      = 2,      //Typical:2.1V
    LVD_THR_3      = 3 //MIN //Typical:1.7V

} lvd_threshold_t;


void lvd_powerup_enable();
void lvd_powerup_disable();
void lvd_threshold_set(lvd_threshold_t lvd_thr);
void lvd_init();
#endif   //__LVD_H_