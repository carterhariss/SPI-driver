/*
 * BME280 barometer driver
 *
 * SPI Mode 0 (CPOL=0, CPHA=0), max 10 MHz
 *
 * Pins:
 *   CSB - chip select (active low)
 *   SCK - serial clock
 *   SDI - data in  (MOSI)
 *   SDO - data out (MISO)
 *
 * Address byte format (first byte of every transaction):
 *   bit 7   : R/W — 0 = write, 1 = read
 *   bits 6:0: register address
 */

#ifndef BME280_H
#define BME280_H

#include <stdint.h>

#endif /* BME280_H */
