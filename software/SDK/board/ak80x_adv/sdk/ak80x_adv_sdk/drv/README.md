﻿
# 简介

> AK801 SDK更新记录。

# 快速开始

参考AK801用户手册。

# 版本更新
## V2.0.4(2021/05/21)
- 使用使用retention Ram(地址0x6000000C)记忆芯片版本信息，0x55:M版 ;0xAA:H版;
- 修复使用寄存器0x40120124记忆芯片版本时出现的问题：GPIO0 不能接外部电压。
## V2.0.3(2021/05/15)
- 使用寄存器0x40120124的BIT[6]记忆芯片版本信息，1:M版 ;0:H版;
- 修复工作时CPU时钟选择和timer时钟选择，避免选用工作时钟32kHZ。
## V2.0.2(2021/03/02)

- 兼容ECO版芯片驱动代码及测试代码，第一次上电时读寄存器(0x4012018C)，根据寄存器值(0x1C:ECO;0x18:非ECO)来自动判断芯片为ECO版或非ECO版。
- 使用RETENTION memery(使用地址0x6000001C)记录ECO或非ECO标志，若芯片为ECO,RETENTION memery写为0x55,否则为0xAA。

## V2.0.1(2021/02/26)

- 兼容ECO版芯片驱动代码及测试代码,使用编译宏选择ECO版或非ECO版

## V1.0.54(2021/01/21)

- 修改接收频率(ISM频段外)对应射频通道的API。

## 更早版本
...

