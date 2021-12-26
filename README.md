# Hello Wifi and Mqtt

ESP-IDF skeleton project with Wifi+Mqtt settings flow (access point with captive portal to get wifi and mqtt settings before going into station mode).

* Flash device with `idf.py build flash monitor`. 
* On smartphone, connect to "klankentapper" access point with password "klankentapper"
* A captive portal appears.
* Provide MQTT settings and save
* Select WiFi to connect to and provide password, device will reset and connect
* Visit device's IP address with browser to update settings