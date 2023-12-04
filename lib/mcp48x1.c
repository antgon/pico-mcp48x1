/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcp48x1.h"

void mcp48x1_set_gain(enum mcp48x1_gain gain, mcp48x1_dac_t *dac) {
    switch (gain) {
    case MCP48X1_GAIN_1X:
        dac->gain_bit = 1 << 13;
        dac->gain_val = 1.0;
        break;
    case MCP48X1_GAIN_2X:
        dac->gain_bit = 0;
        dac->gain_val = 2.0;
        break;
    }
}

void mcp48x1_init(spi_inst_t *spi_port, uint8_t spi_pin_cs,
                  uint8_t spi_pin_mosi, uint8_t spi_pin_sck,
                  enum mcp48x1_resolution resolution,
                  mcp48x1_dac_t *dac) {
    dac->spi_port = spi_port;
    dac->spi_pin_cs = spi_pin_cs;
    dac->spi_pin_mosi = spi_pin_mosi;
    dac->spi_pin_sck = spi_pin_sck;   
    dac->resolution = resolution;

    // Set gain 1x by default
    mcp48x1_set_gain(MCP48X1_GAIN_1X, dac);

    // Initialise SPI pins. This must take place after initialising SPI
    // with `spi_init()` but that is probably best left to the main
    // program in case there are other SPI devices also in use.
    // Baudrate: data sheet p.6, Max is 20 MHz
    spi_set_format(dac->spi_port, MCP48X1_SPI_DATA_BITS,
                   SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(dac->spi_pin_mosi, GPIO_FUNC_SPI);
    gpio_set_function(dac->spi_pin_sck, GPIO_FUNC_SPI);
    gpio_init(dac->spi_pin_cs);
    gpio_set_dir(dac->spi_pin_cs, GPIO_OUT);
    gpio_put(dac->spi_pin_cs, 1);
}

void mcp48x1_cs_select(mcp48x1_dac_t *dac) {
    // These cs_select/deselect functions were taken from pico-examples.
    // The original examples use assembler instructions (ams) to pull
    // the CS pin high or low but these may not really be needed, see
    // https://forums.raspberrypi.com/viewtopic.php?t=332078 asm
    // volatile("nop \n nop \n nop");
    gpio_put(dac->spi_pin_cs, 0);
    // asm volatile("nop \n nop \n nop");
}

void mcp48x1_cs_deselect(mcp48x1_dac_t *dac) {
    // asm volatile("nop \n nop \n nop");
    gpio_put(dac->spi_pin_cs, 1);
    // asm volatile("nop \n nop \n nop");
}

void mcp48x1_put(uint16_t value, mcp48x1_dac_t *dac) {
    // The write command is 16 bits: 4 for configuration (bits 15-12)
    // and 12 for data (bits 11-0).
    //
    // For 12-bit DAC, Write command register is
    // 15, 0 = Write to DAC, 1 = Ignore this command
    // 14, (don't care)
    // 13, ~GA (Gain selection: 1 = 1x, 0 = 2x)
    // 12, ~SHDN 0 = shutdown the device, 1 = device is active
    // 11-0, DAC input data bits
    //
    // If 10-bit DAC, data is bits 11-2; bits 1-0 are ignored
    // If 8-bit DAC, data is bits 11-4; bits 3-0 are ignored
    switch (dac->resolution) {
        // If  8-bit DAC, value &= 0xff then left shift by 4
        case MCP48X1_RESOLUTION_8:
            value &= 0xff;
            value <<= 4;
            break;
        // If 10-bit DAC, value &= 0x3ff then left shift by 2
        case MCP48X1_RESOLUTION_10:
            value &= 0x3ff;
            value <<= 2;
            break;
        // If 12-bit DAC, value &= 0xfff
        case MCP48X1_RESOLUTION_12:
            value &= 0xfff;
            break;
    }
    uint16_t buf = (MCP48X1_WRITE | dac->gain_bit |
                    MCP48X1_DEVICE_NOT_SHUTDOWN | value);
    mcp48x1_cs_select(dac);
    spi_write16_blocking(dac->spi_port, &buf, 1);
    mcp48x1_cs_deselect(dac);
}

void mcp48x1_put_voltage(float voltage,  mcp48x1_dac_t *dac) {
    // The DAC input code (D) is calculated (data sheet eq. 4-1) as
    //     D = Vout/G * (2^n/VREF)
    //
    // where 
    //     VREF = internal voltage reference, 2.048 V
    //     G = gain selection (1 or 2)
    //     n = DAC resolution (8, 10, or 12)
    //
    // The constants used below to calculate D are (2^n/VREF), e.g. for
    // 12-bit DAC, 2^12/VREF = 2^12/2.048 = 2000
    //
    // Perhaps it would be good to define these elsewhere
    static uint16_t dac_val;
    switch (dac->resolution) {
    case MCP48X1_RESOLUTION_8:
        dac_val = (uint16_t)(voltage / dac->gain_val * 125.0);
        break;
    case MCP48X1_RESOLUTION_10:
        dac_val = (uint16_t)(voltage / dac->gain_val * 500.0);
        break;
    case MCP48X1_RESOLUTION_12:
        dac_val = (uint16_t)(voltage / dac->gain_val * 2000.0);
        break;
    }
    mcp48x1_put(dac_val, dac);
}