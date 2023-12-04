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

// Repeating timer for setting the DAC's input code at regular
// intervals. With a 50-us interval, each ramp from 0 to 3.3V (digital
// code 0 to 3299) should take 165 ms to complete (i.e. 3300 * 5e-5
// seconds).
const int32_t timer_interval_us = 50;
bool timer_callback(repeating_timer_t *rt);

// DAC-related variables
struct mcp48x1_dac dac;
uint16_t dac_input = 0;
// With a gain of 2x (set below), 3300 is the maximum valid digital
// code, and corresponds to an analog output of 3.3 V.
const uint16_t dac_max = 3300;

int main() {
    stdio_init_all();

    // Initialise SPI and DAC
    spi_init(DAC_SPI_PORT, DAC_SPI_BAUD);
    mcp48x1_init(DAC_SPI_PORT, DAC_SPI_PIN_CS, DAC_SPI_PIN_MOSI,
                 DAC_SPI_PIN_SCK, MCP48X1_RESOLUTION_12, &dac);
   
    // Set DAC gain to 2x.
    mcp48x1_set_gain(MCP48X1_GAIN_2X, &dac);

    // Start repeating timer
    repeating_timer_t timer;   
    add_repeating_timer_us(-timer_interval_us, timer_callback,
                           NULL, &timer);
    
    while(1) {
        tight_loop_contents();
    }
    return 0;
}

bool timer_callback(repeating_timer_t *rt) {
    mcp48x1_put(dac_input, &dac);
    dac_input++;
    if (dac_input == dac_max) {
            dac_input = 0;
        }
}
