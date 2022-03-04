# ESP soundmeter

## 1. How to measure sound

### General overview

![Flow overview](/documentation/imgs/audio-measure-flow.drawio.png)

### Microphone

We will be using a MEMS microphone, the Infineon IM69D120 to be more specific. Follow [this link](https://github.com/Makerspace-Antwerpen/klankentappers-PCB/blob/master/README.md) for further information on how this microphone works.

We have chosen this microphone based on it's almost flat frequency response out of the box. We still need to filter the signal to get an optimal signal to work with.

![Frequency response](/documentation/imgs/frequency_response.png)

This chip sends out a PDM signal by default. By using the ADAU 7002 converter chip we are able to convert this signal to I2S, which is usable by for example our ESP32 and a variety of other microcontrollers.

### IIR filters

Our microphone, although carefully selected for the flattest possible frequency response, still has a frequency response that isn't completely flat. This is a problem that needs to be sorted before we can use the measurements for anything meaningful.
We also want to apply an A weighting filter to our signal, so the end values represent a DBA measurement. You can read more about the A-weighting of audio signals on [this link](https://en.wikipedia.org/wiki/A-weighting)

To achieve both of those things, we use IIR filters. These are a kind of digital filter that can be applied to a digital signal. You can read more about IIR filters on [this link](https://en.wikipedia.org/wiki/Digital_filter). The filter code used in iir-filter.hpp is universal. It is the filter coefficients that dictate the characteristics of the filter. Acquiring these filter coefficients requires some advanced mathematics that go out of scope for this document. You can find the used filter coefficients in the codebase on line 31-34 of [dbaMeasure.cpp](/main/src/dbaMeasure.cpp). Be advised that these values are sample rate specific and can thus be only used with a 48kHz sample rate.

The first filter used flattens the frequency response of the used microphone. The second filter applies the A-weighting to the audio signal before the RMS is calculated.

### RMS and DBA calculation
We now have a leveled and then weighted audio signal. The next step is calculating a DBA value from this signal. The time interval over which this value is calculated is important. We chose for the fast standard. See [this](https://en.wikipedia.org/wiki/Sound_level_meter#Time_weighting) link for more information. The fast standard measures the sound level over an interval of 1/8th of a second. Therefore, we use 6000 (48000/8) samples for each rms DBA calculation. 
To calculate the RMS value we use the following formula:  
![formula](https://render.githubusercontent.com/render/math?math=RMS=\sqrt{\frac{\sum_{0}^{n}s^2}{n}})  
Where:  
![formula](https://render.githubusercontent.com/render/math?math=n=6000))  
![formula](https://render.githubusercontent.com/render/math?math=s_n=sample_n)  
After this we calculate the DBA value from this RMS value with the following formula:  
![formula](https://render.githubusercontent.com/render/math?math=DBA=MIC\_OFSET\_DB+MIC\_REFF\_DB+\frac{20\log{RMS}}{MIC\_REFF\_AMP})  
Where:  
![formula](https://render.githubusercontent.com/render/math?math=MIC\_OFSET\_DB=) Correction db for dc value of mic. 0 in our case.  
![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_DB=) Sound level at which the ![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_AMP) occurs.  
![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_AMP=) RMS value that occurs at ![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_DB) sound level.  

The ![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_DB) and ![formula](https://render.githubusercontent.com/render/math?math=MIC\_REFF\_AMP) values are experimentally acquired.



## 2. Wifi and MQTT setup

The klankentapper project is a ESP-IDF project with Wifi+Mqtt settings flow (access point with captive portal to get wifi and mqtt settings before going into station mode).

* Flash device with `idf.py build flash monitor`
* On smartphone, connect to "esp32" access point
* A captive portal appears
* Provide MQTT settings and save
* Select WiFi to connect to and provide password, device will reset and connect
* Visit device's IP address with browser to update settings

You can find a comprehensive explanation inside our [documentation readme](/documentation/readme.md).