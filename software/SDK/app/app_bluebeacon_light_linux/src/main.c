#include "board.h"
#include "ty_beacon.h"
#include "app_dps.h"

int main( void ){

	u8 mac[6] = {0xdc,0x23,0x4d,0x5e,0xf4,0x9f};
	u8 authkey[17] = "cdhqA8NQELrVnUYS";
	u8 pid[9] = "chltwaay";
	u8 version = 0x10;//1.0
	u16 kind = 0x0103;

    ty_beacon_init(mac,authkey,pid,version,kind);

    if(ty_beacon_get_state() == DEVICE_STATE_NOT_PAIRED)
		ty_beacon_start_pairing();

    while(1){
        app_light_run();
        ty_beacon_run();
    }
}
