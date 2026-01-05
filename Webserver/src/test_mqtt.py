import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    # print("Connected with code:", rc)
    client.subscribe("utils/test")

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload.decode())

# Create client with a custom client ID
client = mqtt.Client(client_id="a0ms1n-mqtt-server")

# Set username/password (if broker requires it)
client.username_pw_set(username="a0ms1n", password="D$N-676769")

client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.hivemq.com", 1883, 60)
client.loop_forever()
