/*************************************************************************
	> File Name: bal_beacon.c
	> Author: 
	> Mail: 
	> Created Time: Sat 19 Dec 2020 23:48:08 CST
 ************************************************************************/

#include "bal_beacon.h"

//BLE ADV_data, maxlen=31
//#define LEN_DATA 31
u8 tx_buf[31] = {30,0xff,
    0x07, 0xd0,//company id
    0x00,//head
    0xDC,0x23,0x4C,0x00, 0x00,0x01,//mac
    0x00,0x00,0x00,0x01,//sn
    0x00,0x00,0x00,0x01,//sn'
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,//params'
    0x00,//crc-8
    0x00
};//transmit-params
u8 rx_buf[39]; //include header(2B)+mac(6B)+data(max31B), for rx application

static int bluez_beacon_start(void);
static void bluez_beacon_send(u8* tx_buf, u8 tx_buf_len);//tx_buf_len <= 31
void bal_beacon_txrx(u32 tx_cnt, u32 rx_cnt){
	//beacon_txrx(tx_cnt,rx_cnt);
    bluez_beacon_start();
    if(tx_cnt != 0){
        bluez_beacon_send(tx_buf,31);
    }
}


///////////////////////////////////////////////////////////////////
//下面时linux调用Bluez的内部函数，为了满足bal_beacon_txrx函数
///////////////////////////////////////////////////////////////////
static volatile int signal_received = 0;
static void sigint_handler(int sig)
{
	signal_received = sig;
}

static int check_report_filter(uint8_t procedure, le_advertising_info *info)
{
	uint8_t flags;

	/* If no discovery procedure is set, all reports are treat as valid */
	if (procedure == 0 && info->length == 31)
		return 1;

	return 0;
}

static int print_advertising_devices(int dd, uint8_t filter_type)
{
	unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr;
	struct hci_filter nf, of;
	struct sigaction sa;
	socklen_t olen;
	int len;

	olen = sizeof(of);
	if (getsockopt(dd, SOL_HCI, HCI_FILTER, &of, &olen) < 0) {
		printf("Could not get socket options\n");
		return -1;
	}

	hci_filter_clear(&nf);
	hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
	hci_filter_set_event(EVT_LE_META_EVENT, &nf);

	if (setsockopt(dd, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0) {
		printf("Could not set socket options\n");
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_NOCLDSTOP;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	while (1) {
		evt_le_meta_event *meta;
		le_advertising_info *info;
		char addr[18];

		while ((len = read(dd, buf, sizeof(buf))) < 0) {
			if (errno == EINTR && signal_received == SIGINT) {
				len = 0;
				goto done;
			}

			if (errno == EAGAIN || errno == EINTR)
				continue;
			goto done;
		}

		ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
		len -= (1 + HCI_EVENT_HDR_SIZE);

		meta = (void *) ptr;

		if (meta->subevent != 0x02)
			goto done;

		/* Ignoring multiple reports */
		info = (le_advertising_info *) (meta->data + 1);
		if (check_report_filter(filter_type, info)) {
			char name[30];

			memset(name, 0, sizeof(name));

			ba2str(&info->bdaddr, addr);
			//eir_parse_name(info->data, info->length, name, sizeof(name) - 1);
			//printf("%s %s\n", addr, name);

            //for(int i=0;i<info->length;i++){
            //    printf("%02X ",info->data[i]);
            //}
            //printf("\r\n");

            memcpy(&rx_buf[8],info->data,info->length);
            extern u8 ty_beacon_download(u8 *rx_buf, u8 len_pdu, u8 rssi);
            ty_beacon_download(rx_buf,info->length+8,0);
		}
	}

done:
	setsockopt(dd, SOL_HCI, HCI_FILTER, &of, sizeof(of));

	if (len < 0)
		return -1;

	return 0;
}

static void cmd_lescan(int dev_id)
{
	int err, opt, dd;
	uint8_t own_type = LE_PUBLIC_ADDRESS;
	uint8_t scan_type = 0x01;
	uint8_t filter_type = 0;
	uint8_t filter_policy = 0x00;
	uint16_t interval = htobs(0x0010);
	uint16_t window = htobs(0x0010);
	uint8_t filter_dup = 0x00;//--duplicates


	if (dev_id < 0)
		dev_id = hci_get_route(NULL);

	dd = hci_open_dev(dev_id);
	if (dd < 0) {
		perror("Could not open device");
		exit(1);
	}

	err = hci_le_set_scan_parameters(dd, scan_type, interval, window,
						own_type, filter_policy, 10000);
	if (err < 0) {
		perror("Set scan parameters failed");
		exit(1);
	}

	err = hci_le_set_scan_enable(dd, 0x01, filter_dup, 10000);
	if (err < 0) {
		perror("Enable scan failed");
		exit(1);
	}

	printf("LE Scan ...\n");

	err = print_advertising_devices(dd, filter_type);
	if (err < 0) {
		perror("Could not receive advertising events");
		exit(1);
	}

	err = hci_le_set_scan_enable(dd, 0x00, filter_dup, 10000);
	if (err < 0) {
		perror("Disable scan failed");
		exit(1);
	}

	hci_close_dev(dd);
}

//hciconfig hci0 reset
static void hci0_reset(void){
    int ctl;
    struct hci_dev_info di;
    
    /* Open HCI socket  */
	if ((ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI)) < 0) {
		perror("Can't open HCI socket.");
		exit(1);
	}

    di.dev_id = 0;//hci0

   	if (ioctl(ctl, HCIGETDEVINFO, (void *) &di)) {
		perror("Can't get device info");
		exit(1);
	}

    /* Stop HCI device */
	if (ioctl(ctl, HCIDEVDOWN, di.dev_id) < 0) {
		fprintf(stderr, "Can't down device hci%d: %s (%d)\n",
						di.dev_id, strerror(errno), errno);
		exit(1);
	}

    /* Start HCI device */
	if (ioctl(ctl, HCIDEVUP, di.dev_id) < 0) {
		if (errno == EALREADY)
			return;
		fprintf(stderr, "Can't init device hci%d: %s (%d)\n",
						di.dev_id, strerror(errno), errno);
		exit(1);
	}
}

static int bluez_beacon_start(void){
    static pthread_t thid = NULL;
    int dev_id = -1;
    bdaddr_t ba;

    if(thid == NULL){
        hci0_reset();

        if (dev_id != -1 && hci_devba(dev_id, &ba) < 0) {
            perror("Device is not available");
            exit(1);
        }

        if(pthread_create(&thid,NULL,(void *)cmd_lescan,dev_id) != 0){
            PR_DEBUG("thread create error!\n");
            exit(1);
        }
    }
	return 0;
}

static void bluez_beacon_send(u8* tx_buf, u8 tx_buf_len){//tx_buf_len <= 31
    if(tx_buf_len > 31)return;

    static u32 tick = 0;

    if(hal_clock_time_exceed(tick, 30000)){
        tick = hal_clock_get_system_tick();

        char send_buf[] = "btmgmt add-adv -c -d                                                                  &> /dev/null";

        unsigned char *pin = tx_buf;
        const char * hex = "0123456789ABCDEF";
        char * pout = &send_buf[21];
        for(int i = 0; i < tx_buf_len; i++){
            *pout++ = hex[(*pin>>4)&0xF];
            *pout++ = hex[(*pin++)&0xF];
        }
        *pout++ = ' ';
        *pout++ = '1';

        //printf("%s\r\n",send_buf);
        system(send_buf);
    }
}
