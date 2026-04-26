#include "imu.h"
#include "imu_hw.h"


int bno085_reset(void){
    bno085_hw_reset_low();
    bno085_delay_ms(BNO085_RESET_HOLD_TIME_MS); // delay for 10 ms
    bno085_hw_reset_high();
    bno085_delay_ms(BNO085_BOOT_TIME_MS); // delay for 100 ms
    return BNO085_OK;
}

int bno085_init(void){
    //
}
