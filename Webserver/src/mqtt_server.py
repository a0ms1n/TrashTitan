import asyncio
from config import *
import paho.mqtt.client as mqtt

client = mqtt.Client(client_id=MQTT_CONFIG._client_id)

def on_connect(client, userdata, flags, rc):
    client.subscribe("utils/test")

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload.decode())

def start_server():
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect_async(*MQTT_CONFIG._broker)
    client.loop_start()
    print("MQTT Connected!")

def stop_server():
    client.loop_stop()
