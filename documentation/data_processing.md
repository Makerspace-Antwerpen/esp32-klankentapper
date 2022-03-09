# Data processing

This document possesses some tutorials on how to process/visualize your Klankentapper data via a Raspberry Pi. We will provide links to the original websites if you want to setup these applications yourself.

![Visualisation](/documentation/imgs/visualisation.png)

## MQTT Broker

In order to be able to subscribe to your set topic you will need a MQTT broker. We will be using the well-known [Mosquitto Broker](https://mosquitto.org/).

## Database

We will store our data on a database, here we will be using the open source, time series based [InfluxDB](https://www.influxdata.com/).

## Processing

Storing or data is fun and all but what about being able to perceive said data? We will be using two different application for this, [Grafana](https://grafana.com/) and [Node-Red](https://nodered.org/).

## Visualisation

### Node-red

![Node-red](/documentation/imgs/node_red.png)

The most important role for Node-red in our setup is getting our Klankentapper data to InfluxDB by subscribing to our set topic, converting incoming data to usable values and then pushing it to our database.

Add in a "MQTT In" node and fill in your topic:

![Node-red](/documentation/imgs/mqtt_node.png)

The next step is to add a server inside the "MQTT In" node and fill in your MQTT broker ip:

![Node-red](/documentation/imgs/mqtt_node_server.png)

![Node-red UI](/documentation/imgs/node_red_ui.png)

### Grafana

Grafana is used for converting our data into a visual graph, which makes monitoring/viewing our data much easier.

![Grafana](/documentation/imgs/grafana.png)

The only thing we will need is the InfluxDB plugin inside our Grafana web interface:
