# Data processing

This document possesses some tutorials on how to process/visualize your Klankentapper data via a Raspberry Pi. We will provide links to the original websites if you want to setup these applications yourself.

![Visualisation](/documentation/imgs/visualisation.png)

## MQTT Broker

The most important part about MQTT is setting up our broker, this broker will act as a post office. Our ESP32 will send messages to a topic on said broker. Finally we will be able to subscribe to this topic with our client of choice and receive these messages. We will be using the well-known [Mosquitto Broker](https://mosquitto.org/).

The installation is rather simple when you are using a Raspberry Pi (raspbian OS) to host the MQTT broker. Refer to the above mentioned Mosquitto documentation if you want to host your broker on another platform.

### 1. Update your OS

Before installing the MQTT broker to our Raspberry Pi, we need to update the operating system. All we need to do to update the system is to run the following two commands:

    sudo apt update
    sudo apt upgrade

### 2. Installing the Mosquitto software

Luckily for us, the Mosquitto MQTT broker is available as part of the Raspbian repository, so installing the software is simple. Run the following command to install Mosquitto alongside its client software:

    sudo apt install mosquitto mosquitto-clients

### 3. Checking installation

At this point, you will now have the Mosquitto MQTT broker up and running on your device. You can verify that it is installed and running by using the command below:

    sudo systemctl status mosquitto

### 4. Running on boot

Make your Mosquitto broker start on boot by using the following command:

    sudo systemctl enable mosquitto.service

## Database

We would like to store our data somewhere so that we can retrieve/review it later, this is why we will be using a database. For this example we will show you how to utilize the open source, time series based [InfluxDB](https://www.influxdata.com/).

Please refer to [this tutorial](https://pimylifeup.com/raspberry-pi-influxdb/) to set up your own InfluxDB database.

## Data Transportation

InfluxDB cannot subscribe to our topic on its own, this is why we need to help transport our data to our database. We will show you 2 ways on how to do this:

### Node-red flow

![Node-red UI](/documentation/imgs/node_red_flow.png)

This Nod-red flow ensures all our Klankentapper data will get to our database. We only need 3 nodes to be establish this flow:

#### *MQTT in node*

The first step is to add a server inside the "MQTT In" node and fill in your MQTT broker ip:

<img src="imgs/mqtt_node_server.png" alt="MQTT in server" style="width:500px;"/>

Next fill in your topic and set the output to "a parsed JSON object":

<img src="imgs/mqtt_node.png" alt="MQTT in settings" style="width:500px;"/>

#### *JSON to float node*

This "function node" will serve as a JSON string to float converter. This step is important if you want to visualize your data as a time series graph. Just copy in the following code:

    msg.payload = Number(msg.payload.dBA);
    return msg;

Now it should look like this:

<img src="imgs/node_red_float.png" alt="JSON to float" style="width:500px;"/>

#### *InfluxDB node*

Finally we will configure our "InfluxDB out" node. First we will need to add InfluxDB support to our Node-red. Use the [palette manager](https://nodered.org/docs/user-guide/editor/palette/manager) and install the "node-red-contrib-influxdb" palette.

The first step is to add a server inside the "InfluxDB out" node, fill in your database URL and be sure to add the credentials of an **admin** user.

**Attention!** Be sure to check your InfluxDB version and adjust if necessary!

<img src="imgs/influxdb_node_server.png" alt="InfluxDB out server" style="width:500px;"/>

Next fill in your database and give your measurement a name:

<img src="imgs/influxdb_node.png" alt="InfluxDB out settings" style="width:500px;"/>

Done! Don't forget to deploy! Now you are able to send data from your MQTT broker to your database.

### Python script

An other way to send data is via a python script. This python script is able to send data from your broker to your database while running in the background of your Raspberry Pi.

**Attention!** In order to run this script you will need [Python3](https://projects.raspberrypi.org/en/projects/generic-python-install-python3) and [Paho MQTT](https://pypi.org/project/paho-mqtt/).

First thing to do is make a new python script in your user directory (`cd ~/`) by making a new script, `nano my_new_script.py` and adding the following code:

    from paho.mqtt import client as mqtt_client
    from influxdb import InfluxDBClient
    import json

    broker = 'broker_ip'
    port = 1883
    topic = "your/topic"
    client_id = f'a_client_id'

    influxdb = 'influxdb_ip'
    user = 'amdin_user'
    password = 'admin_password'
    database = 'your_database'

    influxdb_client = InfluxDBClient(influxdb, 8086, user, password, None)
    influxdb_client.switch_database(database)

    def connect_mqtt() -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)
        client.on_connect = on_connect
        client.connect(broker, port)
        return client

    def subscribe(client: mqtt_client):
        def on_message(client, userdata, msg):
            print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
            data = json.loads( msg.payload.decode() )
            value = float( data["dBA"] )
            json_body = [
                {
                    'measurement': "measurement_name",
                        'fields': {
                            'value': value
                        }
                }
            ]
            influxdb_client.write_points(json_body)

        client.subscribe(topic)
        client.on_message = on_message


    def run():
        client = connect_mqtt()
        subscribe(client)
        client.loop_forever()


    if __name__ == '__main__':
        run()

The final step is to make this script run on boot. By following [these instructions](https://www.instructables.com/Raspberry-Pi-Launch-Python-script-on-startup/) you are able to accomplish that.

## Data Visualisation

Storing our data is fun and all but what about being able to perceive said data? We will be using two different application for this, [Grafana](https://grafana.com/) and [Node-Red](https://nodered.org/).

### Node-red UI

Aside from transporting data from our MQTT broker to our database we are also able to visualize said data inside Node-red. We will be using Node-red dashboard, [this tutorial](https://flows.nodered.org/node/node-red-dashboard) shows all you need to know about Node-red dashboard.

#### *Example*

As an example we will show you how we can visualize our data as a gauge:

1. Add a "gauge node" from the left menu inside your flow

<img src="imgs/node_red_ui_nodes.png" alt="Add gauge node" style="width:200px;"/>

2. Connect your "JSON to float" node to this node

<img src="imgs/gauge_node.png" alt="Add gauge node" style="width:250px;"/>

3. Configure the Gauge node accordingly

<img src="imgs/gauge_settings.png" alt="Add gauge node" style="width:300px;"/>

All done!

![Node-red UI](/documentation/imgs/node_red_ui.png)

### Grafana

Grafana is used for converting our data into a time series based graph, which makes monitoring/viewing our data much easier. The only thing we will need is the InfluxDB data source inside our Grafana web interface (under configuration):

![Grafana settings](/documentation/imgs/grafana_data_source.png)

Next thing we need to do is configure our influxDB data source settings according to our database:

* **URL**: This is the ip address of your Raspberry Pi or local computer where InfluxDB is running.
* **Access**: Just select "server (default)".
* **Database**: The name of your database inside InfluxDB.
* **HTTP method**: Select GET.
* **User** and **Password**: When you enable HTTP authentication, InfluxDB requires you to create at least one admin user before you can interact with the system.

Next when all settings are done we need to add a panel to our grafana dashboard of choice:

![Grafana](/documentation/imgs/grafana_panel.png)

Finally we are able to add our time series graph by editing our query settings accordingly:

**Attention!** `Select measurement` needs to be adjusted according to the name you have given your measurement inside your database. You will know when something is wrong when your measurement does not show up inside this setting.

![Query settings](/documentation/imgs/query_settings.png)

Apply the changes to your panel and be sure to add the auto-refresh setting in the top right corner of your dashboard. This way your panels will automatically update.

![Grafana dashboard](/documentation/imgs/grafana.png)