# Klankentapper software

In order to utilize the klankentapper project we will need ESP-IDF. ESP-IDF is a powerfull development framework for building and flashing firmware onto an ESP32 board.

## Installing ESP-IDF

![ESP-IDF](/documentation/imgs/esp_idf_get_started.png)

By following [this link](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) you will be able to download ESP-IDF. Click on the link of your operating system and follow the installation steps. Do this until you reach the step "First Steps on ESP-IDF." You can finish reading the documentation but we do not need to start our own project.

![ESP-IDF download](/documentation/imgs/esp_idf_download.png)

The ESP32 boards are built and flashed by default. The next steps will explain how you can build and flash the klankentapper project on your own but if you only need to flash/update the firmware you can skip to [this step](#flashing-the-esp32).

## Building the project

When you have finished installing all the prerequisites we are able to move on to building the klankentapper project. if you haven't already downloaded the project do so now. Be sure the project is fully accessible by extracting it if downloaded via zip.

Now open the Executable.

<img src="imgs/esp_idf.png" alt="ESP-IDF-1" style="width:600px;"/>

The next step is to navigate to the directory of the project by using the cd command. E.g. when you extracted the zip to your desktop on Windows the command would be: `cd C:\Users\user_name\Desktop\esp32-klankentapper`

If you are in the right directory, you are now able to build the project by using the following command: `idf.py build`

## Flashing the ESP32

In this step we will explain how you can flash and update firmware to your ESP32.

First you will need to open the ESP-IDF executable.

<img src="imgs/esp_idf.png" alt="ESP-IDF-2" style="width:600px;"/>

Navigate to the directory of the project by using the cd command. E.g. when you extracted the zip to your desktop on Windows the command would be: `cd C:\Users\user_name\Desktop\esp32-klankentapper`

If you are in the right directory, you are now able to build the project by using the following command: `idf.py flash`

## Setting up the Wifi

[![Wifi setup](/documentation/imgs/wifi_setup.png)](connectivity.md)