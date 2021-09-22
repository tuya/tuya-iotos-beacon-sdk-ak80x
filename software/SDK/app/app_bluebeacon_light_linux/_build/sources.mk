############################################################
# 全局变量
############################################################
INCS :=
OBJS := 
C_SRCS :=


PROJECT_ROOT := ../../..
BIN = .


#设置最后目标文件
TARGET = app_main  
BIN_TARGET = ${BIN}/${TARGET}  

#编译选项
CC = gcc   
CFLAGS = -g -Wall    
CMACROS = -D LINUX_SYS
