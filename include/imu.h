
#ifndef BNO085_H
#define BNO085_H

#include <stdint.h>

// Status codes
#define BNO085_OK 0
#define BNO085_ERROR -1
#define BNO085_TIMEOUT -2
#define BNO085_RESET_HOLD_TIME_MS 10
#define BNO085_BOOT_TIME_MS 100 

//initialization and setup

int bno085_init(void);
int bno085_reset(void);
int bno085_check_device(void);   // verify communication / device ID


/**
 * bno085_recieve_packet 
 * BNO085 sends data as SHTP packets over SPI
 * args:
 *  - buffer a pointer to a byte array where the enitre packet will be stored.
 * after calling the fucniton the buffer will contain the starting points of the eniter pack
 * - len is a pointer to a variable where the lenght of the pack will be stored
 * - channel is a pointer to a varibale where we store the channel number of the packet
 * this matters becasue the BNO085 uses multiple logical channels, so the channle tells us what type of packet we have
**/
 

// SHTP Packet Handling 
int bno085_receive_packet(uint8_t* buffer, uint16_t* len, uint8_t* channel);




#endif // BNO085_H