/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "mcp48x1.h"

#define DAC_SPI_PORT spi0
#define DAC_SPI_BAUD 1 * 1000 * 1000 // 1 MHz
#define DAC_SPI_PIN_MOSI 3
#define DAC_SPI_PIN_CS 5
#define DAC_SPI_PIN_SCK 6

struct mcp48x1_dac dac;

float voltage[] = {0, 1.65, 3.3};
uint8_t step = 0;

int main() {
    stdio_init_all();

    // Initialise SPI and DAC
    spi_init(DAC_SPI_PORT, DAC_SPI_BAUD);
    mcp48x1_init(DAC_SPI_PORT, DAC_SPI_PIN_CS, DAC_SPI_PIN_MOSI,
                 DAC_SPI_PIN_SCK, MCP48X1_RESOLUTION_12, &dac);
   
    // Set DAC gain to 2x.
    mcp48x1_set_gain(MCP48X1_GAIN_2X, &dac);
    
    while(1) {
        // Set DAC output to each of the values defined above, wait for
        // 4 seconds, then step to the next value.
        mcp48x1_put_voltage(voltage[step], &dac);
        step++;
        if (step == 3) step = 0;
        sleep_ms(4000);
    }
    return 0;
}
