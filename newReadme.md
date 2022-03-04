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
Our microphone, although carefully selected for the flattest possible frequency response, still has a frequency response that isn't completely flat. This is a problem that needs to be sorted before we can use the measurements for anything meaningful.
We also want to apply an A weighting filter to our signal, so the end values represent a DBA measurement. You can read more about the A-weighting of audio signals on: https://en.wikipedia.org/wiki/A-weighting

To achieve both of those things, we use IIR filters. These are a kind of digital filter that can be applied to a digital signal. You can read more about IIR filters on: https://en.wikipedia.org/wiki/Digital_filter . The filter code used in iir-filter.hpp is universal. It is the filter coefficients that dictate the characteristics of the filter. Acquiring these filter coefficients requires some advanced mathematics that go out of scope for this document. You can find the used filter coefficients in the codebase on line 31-34 of "dbaMeasure.cpp". Be advised that these values are sample rate specific and can thus be only used with a 48kHz sample rate.

The first filter used flattens the frequency response of the used microphone. The second filter applies the A-weighting to the audio signal before the RMS is calculated.

### RMS and DBA calculation

## 2. Wifi and MQTT setup
