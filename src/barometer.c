#include "barometer.h"
#include <stdint.h>

/* TODO: replace with SPI instance and CS pin from STM32 */
#define BME280_SPI       SPI1
#define BME280_CS_PORT   GPIOA
#define BME280_CS_PIN    (1U << 4)   /* PA4 (Port A, Pin 4) TODO: update */

/* SPI helpers */

static void cs_low(void) {
    BME280_CS_PORT->ODR &= ~BME280_CS_PIN;   /* pull CS low to start transaction */
}

static void cs_high(void) {
    BME280_CS_PORT->ODR |= BME280_CS_PIN;    /* pull CS high to end transaction */
}

static uint8_t spi_transfer(uint8_t byte) {
    while (!(BME280_SPI->SR & (1U << 1)));   /* wait until transmit buffer empty (TXE) */
    BME280_SPI->DR = byte;                   /* send byte */
    while (!(BME280_SPI->SR & (1U << 0)));   /* wait until receive buffer has data (RXNE) */
    return BME280_SPI->DR;                   /* read and return received byte */
}

static uint8_t spi_read(uint8_t addr) {
    uint8_t data;
    cs_low();
    spi_transfer(addr | 0x80);   /* set bit 7 to indicate read */
    data = spi_transfer(0x00);   /* send dummy byte, clock in response */
    cs_high();
    return data;
}

static void spi_write(uint8_t addr, uint8_t data) {
    cs_low();
    spi_transfer(addr & 0x7F);   /* clear bit 7 to indicate write */
    spi_transfer(data);
    cs_high();
}

/* ── Register addresses (Page 27, Table 18) ─────────────────────────────────────── */
#define BME280_REG_ID           0xD0
#define BME280_REG_RESET        0xE0
#define BME280_REG_CTRL_HUM     0xF2
#define BME280_REG_STATUS       0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_REG_CONFIG       0xF5
#define BME280_REG_PRESS_MSB    0xF7
#define BME280_REG_PRESS_LSB    0xF8
#define BME280_REG_PRESS_XLSB   0xF9
#define BME280_REG_TEMP_MSB     0xFA
#define BME280_REG_TEMP_LSB     0xFB
#define BME280_REG_TEMP_XLSB    0xFC
#define BME280_REG_HUM_MSB      0xFD
#define BME280_REG_HUM_LSB      0xFE
#define BME280_REG_CALIB00      0x88  /* calibration data bank 1: 0x88-0xA1 */
#define BME280_REG_CALIB26      0xE1  /* calibration data bank 2: 0xE1-0xF0 */

/* ── Calibration coefficients (Table 16) ───────────────────────────────── */
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} BME280_CalibData;

static BME280_CalibData calib;

/* ── Register values ────────────────────────────────────────────────────── */
#define BME280_CHIP_ID          0x60
#define BME280_SOFT_RESET       0xB6

/* ctrl_meas (0xF4): (temperature) osrs_t=1x (bits 7:5), (pressure) osrs_p=1x (bits 4:2), mode=normal (bits 1:0)
   must write ctrl_hum before ctrl_meas or humidity oversampling setting is ignored */
#define BME280_OSRS_1X          0x01
#define BME280_MODE_NORMAL      0x03
#define BME280_CTRL_MEAS_VAL    ((BME280_OSRS_1X << 5) | (BME280_OSRS_1X << 2) | BME280_MODE_NORMAL)
