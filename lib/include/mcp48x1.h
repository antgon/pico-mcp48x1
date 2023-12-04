/*
 * Copyright (c) 2023 Antonio González
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCP48X1_H
#define MCP48X1_H

#include "pico.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

/** \file mcp48x1.h
 * 
 * \brief Library for using an MCP48x2 digital-to-analogue converter
 * with the Raspberry Pi Pico
 * 
*/

#define MCP48X1_SPI_DATA_BITS 16u
#define MCP48X1_WRITE 0x0000
#define MCP48X1_DEVICE_SHUTDOWN 0x0000
#define MCP48X1_DEVICE_NOT_SHUTDOWN 1 << 12
#define MCP48X1_VREF 2.048f

enum mcp48x1_gain {
    MCP48X1_GAIN_1X,
    MCP48X1_GAIN_2X
};

enum mcp48x1_resolution {
    MCP48X1_RESOLUTION_8,
    MCP48X1_RESOLUTION_10,
    MCP48X1_RESOLUTION_12
};

typedef struct mcp48x1_dac {
    spi_inst_t *spi_port;
    uint8_t spi_pin_cs;
    uint8_t spi_pin_mosi;
    uint8_t spi_pin_sck;
    uint16_t gain_bit;
    float gain_val;
    enum mcp48x1_resolution resolution;
} mcp48x1_dac_t;

void mcp48x1_init(spi_inst_t *spi_port, uint8_t spi_pin_cs,
                  uint8_t spi_pin_mosi, uint8_t spi_pin_sck,
                  enum mcp48x1_resolution resolution,
                  mcp48x1_dac_t *dac);

void mcp48x1_cs_select(mcp48x1_dac_t *dac);

void mcp48x1_cs_deselect(mcp48x1_dac_t *dac);

void mcp48x1_set_gain(enum mcp48x1_gain gain, mcp48x1_dac_t *dac);

void mcp48x1_put(uint16_t value, mcp48x1_dac_t *dac);

void mcp48x1_put_voltage(float voltage,  mcp48x1_dac_t *dac);

#endif
