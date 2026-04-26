#ifndef BNO085_HW_H
#define BNO085_HW_H

#include <stdint.h>

 void bno085_hw_reset_low(void);
 void bno085_hw_reset_high(void);
 void bno085_delay_ms(uint32_t ms);

 #endif