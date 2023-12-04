# Sawtooth analog signal

Using a MCP48x1 DAC with the Raspberry Pico, this code outputs a
sawtooth analog waveform. To observe the signal connect the DAC's output
pin (pin 8) to an oscilloscope.

A MPC4821 12-bit DAC is assumed for this example, but only minor changes
are needed for using this with 8- or 10-bit MCP48x1 DACs.


## Wiring

Raspberry Pico          | MCP48x1
------------------------|------------
3V3(OUT) (pin 36)       | VDD (pin 1)
GPIO 5 (pin 7) SPI0 CSn | ~CS (pin 2)
GPIO 6 (pin 9) SPI0 SCK | SCK (pin 3)
GPIO 3 (pin 5) SPI0 TX  | SDI (pin 4)
GND (pin 3)             | ~LDAC (pin 5)
3V3(OUT) (pin 36)       | ~SHDN (pin 6)
GND (pin 3)             | VSS (pin 7)


## Additional materials

Capactitors, as recommended in the data sheet (Fig 6-1, p.25):

* MCP48x1 VDD (pin 1): Add decoupling capacitors to ground, 0.1 uF and
  10 uF (tant.)
* MCP48x1 Vout (pin 8): Add capacitor to ground, 1 uF (but see Section
  6.3)
  