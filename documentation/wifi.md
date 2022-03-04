# Klankentapper wifi

## Starting the klankentapper

The first time the ESP32 is provided with power it will act as an access point. You will be able to connect to this access point with your smartphone/computer via the network called "esp32". **Attention!** After connecting to this network you will not be able to connect to the internet for a while!

![Access point](/documentation/imgs/wifi_access_point.png)

After connecting to this network the next browser page will open:

![Browser page](/documentation/imgs/wifi_browser.png)

## Setting up the wifi

Firstly we will setup the wifi. At the bottom of the page you will be able to choose which network your klankentapper will be connected to. Now choose your preferred network and fill in the password.

![Wifi password](/documentation/imgs/wifi_password.png)

This will take a while. You can monitor the connection phase by opening the ESP-IDF executable, moving to the right directory and typing the following command:

    idf.py monitor

When the connection is established you are greeted with an ip-address which we will use to configure our MQTT connection.

![Connected](/documentation/imgs/wifi_connection.png)

Connect to your favourite network, open up your internet browser and type in the ip-address you have monitored in the previous step. this should open up the same browser from before, now with full internet accessibility.

## Connecting with the MQTT broker

Next up is configuring our MQTT connection. The next step will explain how you can setup a MQTT connection with your favourite broker of choice. If you don't have any MQTT broker you can follow our tutorial to set one up via [this link](/documentation/mqtt.md).

Setting up a MQTT connection on your klankentapper is rather simple. You are able to fill in 4 parameters:

- Broker url, Type in mqtt:// followed by your broker ip-address.
- Topic, specify to which topic you want to publish.
- Username
- Password

Both username and password are optional. These settings only need to be filled in when you need to sign in to your broker by using your credentials.

![MQTT settings](/documentation/imgs/mqtt_settings.png)