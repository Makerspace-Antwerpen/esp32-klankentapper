# ESP soundmeter

## 1. How to measure sound

### General overview

afbeelding

### Microphone

We will be using a MEMS microphone, the Infineon IM69D120 to be more specific. Follow [this link](https://github.com/Makerspace-Antwerpen/klankentappers-PCB/blob/master/README.md) for further information on how this microphone works.

We have chosen this microphone based on it's almost flat frequency response out of the box. We still need to filter the signal to get an optimal signal to work with.

![Frequency response](/documentation/imgs/frequency_response.png)

This chip sends out a PDM signal by default. By using the ADAU 7002 converter chip we are able to convert this signal to I2S, which is usable by for example our ESP32 and a variety of other microcontrollers.

### IIR filters

mic freq response flattening

dba weighting

### RMS and DBA calculation

## 2. Wifi and MQTT setup
