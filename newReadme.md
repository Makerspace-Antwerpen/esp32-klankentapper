# ESP soundmeter

## 1. How to measure sound

### General overview

afbeelding

### Microphone

We will be using a MEMS microphone, the Infineon IM69D120 to be more specific. Follow [this link]() for further information on how this microphone works.

### IIR filters
Our microphone, although carefully selected for the flattest possible frequency response, still has a frequency response that isn't completely flat. This is a problem that needs to be sorted before we can use the measurements for anything meaningful.
We also want to apply an A weighting filter to our signal, so the end values represent a DBA measurement. You can read more about the A-weighting of audio signals on: https://en.wikipedia.org/wiki/A-weighting

To achieve both of those things, we use IIR filters. These are a kind of digital filter that can be applied to a digital signal. You can read more about IIR filters on: https://en.wikipedia.org/wiki/Digital_filter . The filter code used in iir-filter.hpp is universal. It is the filter coefficients that dictate the characteristics of the filter. Acquiring these filter coefficients requires some advanced mathematics that go out of scope for this document. You can find the used filter coefficients in the codebase on line 31-34 of "dbaMeasure.cpp". Be advised that these values are sample rate specific and can thus be only used with a 48kHz sample rate.

The first filter used flattens the frequency response of the used microphone. The second filter applies the A-weighting to the audio signal before the RMS is calculated.

### RMS and DBA calculation

bla

## 2. Wifi and MQTT setup
