# Use a MCP48x1 digital-to-analog converter

This code demonstrates how to use a MCP48x1 DAC with the Raspberry Pico.

The DAC's analog output will step from 0 to 1.65 to 3.3V every 4
seconds. These steps should be long enough to allow to test this output
with a multimeter, which is useful if an oscilloscope is not at hand.

These devices come in 3 variants: 8-bit (MCP4801), 10-bit (MCP4811), or
12-bit (MCP4821) resolution. A MPC4821 12-bit DAC is assumed for this
example, but only minor changes are needed for using this with 8- or
10-bit MCP48x1 DACs.


## Wiring

The Pico communicates with the MCP48x1 via SPI. Any SPI pins can thus be
used. For this example, the conections are

Raspberry Pico          | MCP48x1
------------------------|------------
3V3(OUT) (pin 36)       | VDD (pin 1)
GPIO 5 (pin 7) SPI0 CSn | ~CS (pin 2)
GPIO 6 (pin 9) SPI0 SCK | SCK (pin 3)
GPIO 3 (pin 5) SPI0 TX  | SDI (pin 4)
GND (pin 3)             | ~LDAC (pin 5)
3V3(OUT) (pin 36)       | ~SHDN (pin 6)
GND (pin 3)             | VSS (pin 7)

Notes:
* Pin ~CS can be connected to any digital pin in the Pico, does not have
to be `SPI CSn`.
* MCP48x1 pin 8 is Vout, the DAC's analog output.


## Additional materials

Capactitors, as recommended in the data sheet (Fig 6-1, p.25):

* MCP48x1 VDD (pin 1): Add decoupling capacitors to ground, 0.1 uF and
  10 uF (tant.)
* MCP48x1 Vout (pin 8): Add capacitor to ground, 1 uF (but see Section
  6.3)
  