/*************************************************************************
	> File Name: hal_uart.c
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Jun 2018 08:31:59 PM CST
 ************************************************************************/
//ref:https://blog.csdn.net/u012183924/article/details/53560103
//阻塞与非阻塞读取:http://blog.chinaunix.net/uid-23065002-id-3863729.html
//linux下串口的阻塞和非阻塞操作:http://www.cnblogs.com/lidabo/p/4573490.html
#include "hal_uart.h"

//#define DEVICE "/dev/ttyUSB0" 
///////////////////////////////////////////////////////////////////
char usb_str[] = "/dev/ttyUSB_";

#define serial_name_max_size 255

struct serial_list{//串口数据定义
        char serial_name[serial_name_max_size];
        unsigned int serial_device_num; 
};

//检索当前的串口设备
char serial_decice_choose(void)
{
    int list_num = 0;
    char USB_com;
    DIR *dirp; 
    struct dirent *dp;
    struct serial_list Serial_list[5] = {0};

    dirp = opendir("/dev/"); //打开目录指针
    while ((dp = readdir(dirp)) != NULL) { //通过目录指针读目录获取串口信息
        if((dp->d_name[3] == 'U') && (dp->d_name[4] == 'S') && (dp->d_name[5] == 'B')) {
            printf("%s  ", dp->d_name );
            strcpy(Serial_list[list_num].serial_name,dp->d_name);   
            Serial_list[list_num].serial_device_num = dp->d_name[6];
            //printf("Serial_list.serial_name[%d]:%s\n",list_num,Serial_list[list_num].serial_name);
            list_num++;
        } 
    }
    printf("\n");
    if(Serial_list[0].serial_device_num == 0){
        printf("没有发现串口设备！\n");
    }
    if(Serial_list[1].serial_device_num == 0)//只有一个串口默认打开
    {
        USB_com = Serial_list[0].serial_device_num;
    }
    else{
        printf("请输入一位串口号（0~9）：USB");
        USB_com = getchar();
        getchar();//释放缓存区的\n字符
        while((USB_com < '0') || (USB_com > '9')){
            printf("请重新输入一位串口号（0~9）：USB");
            USB_com = getchar();
            getchar();//释放缓存区的\n字符
        }
    }
    return USB_com;
}
//////////////////////////////////////////////////////////////////
int fd = 0;

//打开串口并初始化设置 
int hal_uart_init(void){
    usb_str[11] = serial_decice_choose();
    fd = open(usb_str, O_RDWR | O_NOCTTY | O_NDELAY); 
    if (fd < 0) { 
        perror("open"); 
        return -1; 
    } 

    //串口主要设置结构体termios <termios.h> 
    struct termios options; 

    /**1. tcgetattr函数用于获取与终端相关的参数。 
    *参数fd为终端的文件描述符，返回的结果保存在termios结构体中 
    */ 
    tcgetattr(fd, &options); 
    /**2. 修改所获得的参数*/ 
    options.c_cflag |= (CLOCAL | CREAD);//设置控制模式状态，本地连接，接收使能 
    options.c_cflag &= ~CSIZE;//字符长度，设置数据位之前一定要屏掉这个位 
    options.c_cflag &= ~CRTSCTS;//无硬件流控 
    options.c_cflag |= CS8;//8位数据长度 
    options.c_cflag &= ~CSTOPB;//1位停止位 
    options.c_iflag |= IGNPAR;//无奇偶检验位 
    options.c_iflag &= ~(IXOFF | IXON);//if you want read 0x11 and 0x13
    options.c_oflag = 0; //输出模式 
    options.c_lflag = 0; //不激活终端模式 
    cfsetospeed(&options, B9600);//设置波特率 

    /**3. 设置新属性，TCSANOW：所有改变立即生效*/ 
    tcflush(fd, TCIFLUSH);//溢出数据可以接收，但不读 
    tcsetattr(fd, TCSANOW, &options); 

    return 0; 
} 

/** 
*串口发送数据 
*@datas:待发送数据 
*@length:数据长度 
*/ 
int hal_uart_send(u8 *datas, u8 length){ 
    int i,len = 0; 
    len = write(fd,(char *)datas,(int)length);//实际写入的长度 
    if(len == length) { 
        //LOG-FOR-DEBUG
        PR_DEBUG("%-30s","SEND RAW DATAS:");
        for(i=0;i<len;i++){
            PR_DEBUG_RAW("%02X ",datas[i]&0xFF); 
        }
        PR_DEBUG_RAW("\n");

        return len; 
    } else { 
        tcflush(fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送 
        return -1; 
    } 
    return 0; 
} 

/** 
*串口接收数据 
*要求启动后，在pc端发送ascii文件 
*/
int hal_uart_read(u8 *datas, u8 length){ 
    int len=0, ret = 0; 
    fd_set fs_read; 
    struct timeval tv_timeout; 

    FD_ZERO(&fs_read);//清空串口接收端口集
    FD_SET(fd, &fs_read);//设置串口接收端口集

    tv_timeout.tv_sec = 0; 
    tv_timeout.tv_usec = 10;
    ret = select(fd+1, &fs_read, NULL, NULL, &tv_timeout);

    if(ret == 0 || ret == -1)return -1;//如果返回0，代表在描述符状态改变前已超过timeout时间,错误返回-1 
    
    if (FD_ISSET(fd, &fs_read)) {//检测串口是否有读写动作
        len = read(fd,(char *)datas,(int)length); 
        return len; 
    } else { 
        PR_DEBUG_RAW("UART READ ERROR");
        perror("select"); 
        return -1; 
    } 

    return 0; 
} 

/*
int main(int argc, char **argv){ 
    hal_uart_init(); 

    u8 buf[]="hello world"; 
    u8 buf1[11] ; 
    memset(buf1,0,sizeof(char)*11); 

//    fcntl(fd,F_SETFL,FNDELAY);
//    fcntl(fd,F_SETFL,0);

    while(1){
        hal_uart_send(buf, 11); 
        printf("\n"); 
        usleep(100000);
        if(hal_uart_read(buf1, 11) > 0)
            printf("receive: %s\n", buf1);
        memset(buf1,0,sizeof(char)*11); 
    }

    close(fd); 
    return 0; 
}
*/

