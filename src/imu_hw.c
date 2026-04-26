#include "imu_hw.h"
#include <stdint.h>

void bno085_hw_reset_low(void) {
    printf("Setting reset pin LOW\n");
}

void bno085_hw_reset_high(void) {
    printf("Setting reset pin HIGH\n");
}

void bno085_delay_ms(uint32_t ms) {
    printf("Delaying for %lu ms\n", (unsigned long)ms);
}