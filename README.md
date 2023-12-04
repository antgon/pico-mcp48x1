# pico-mcp48x1

A C library for using a Microchip MCP48x1 digital-to-analogue converter
(DAC) with the Raspberry Pi Pico

The MCP48x1 devices are single-channel DACs interfaced with SPI, and the
family includes MCP4801 (8 bit), MCP4811 (10 bit) and [MCP4821](mcp4821)
(12 bit) DAC. The data sheet is available at Microchip's website
([pdf](datahseet)).

[mcp4821]: https://www.microchip.com/en-us/product/MCP4821

[datasheet]: https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/22244B.pdf


## Wiring

| MCP48x1   | Pico 
----------|---------
(1)  VDD  | 3V3(OUT)
(2) ~CS   | Any GPIO
(3) SCK   | SPIx SCK
(4) SDI   | SPIx TX
(5) ~LDAC | GND
(6) ~SHDN | 3V3
(7) VSS   | GND
(8) VOUT  | -

In addition, the data sheet (Fig 6-1, p.25) recommends adding a few
capacitors:

* VDD (pin 1): Add decoupling capacitors to ground, 0.1 uF and 10 uF
  (tant.)
* Vout (pin 8): Add capacitor to ground, 1 uF (but see data sheet
  Section 6.3)

SPI max baud rate is 20 MHz.


## Digital input to analog output

The analog output voltage is calculated according to this equation (Data
sheet Eq. 4-1):

$$V_{out} = (V_{ref} \times D \times G) / 2^n$$

where
* D = DAC input code
* G = Gain selection (1x or 2x)
* n = Gain resolution (8, 10, or 12)
* Vref = Internal voltage reference, 2.048 V


The analog output, however, cannot be greater than the voltage supplied
to the device [^1]. This means that, when used with the Pico (3V3
supply), the maximum output is < 3.3 V. Thus, depending on the DAC
resolution and the gain setting, the range of valid digital input codes
and their corresponding output voltage is:

| Model   | n  | Gain 1x |        | Gain 2x |        |
|---------|----|--------|----------|--------|---------| 
|         |    | **Input**  | **Output**   | **Input**  | **Output**  |
| MCP4801 |  8 | 0~255  | 0~2.04 V | 0~206  | 0~3.3 V |
| MCP4811 | 10 | 0~1023 | 0~2.05 V | 0~825  | 0~3.3 V |
| MCP4821 | 12 | 0~4095 | 0~2.05 V | 0~3300 | 0~3.3 V |

[^1]: The output swing is in fact 0.01 V to (VDD - 0.04), i.e. 0.01 to
3.26 V for a 3V3 supply (Data sheet Sec. 1).