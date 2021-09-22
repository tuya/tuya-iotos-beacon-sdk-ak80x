/*************************************************************************
	> File Name: hal_sys.h
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Jun 2018 08:31:44 PM CST
 ************************************************************************/

#ifndef _HAL_SYS_H
#define _HAL_SYS_H

#include<stdio.h>
#include<errno.h>
#include<fcntl.h>           //文件控制定义 
#include<unistd.h> 
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<poll.h>
#include<termios.h>         //终端控制定义 
#include<dirent.h>          //文件目录操作
#include<signal.h>          //信号量
#include<time.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>


#include <sys/types.h> 
#include <sys/times.h>
#include <sys/stat.h> 
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <bluetooth/sdp.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


#include "hal_log.h"
#include "hal_types.h"

#endif
