import paho.mqtt.client as mqtt
import json

MQTT_HOST = "localhost" #In actual case IP Addr of MQTT broker. (i.e ip addr of pc which runs mqtt broker)
MQTT_PORT = 1883
MQTT_KEEPALIVE_INTERVAL = 45
MQTT_TOPIC = "payload"

#jsonObject = json.dumps({"locationClientID":{"numberOfBots": 5,"relayNodeID": "Node Thora","performance": "52","activeHours": 4,"businessClientID": "GunapalaRox","locationID":"Thumpane"}});
jsonObject = json.dumps({"BotID": "bot9","ActiveHours": 2,"TriggerData":[{"TimeStamp":"1400","Triggers": 6}]})

#IMPORTANT : REPLACE 'jsonObject' WITH THE JSON OBJECT THAT YOU WANT TO SEND TO THE SERVER.
MQTT_MSG = jsonObject;

# Define on_publish event function
def on_publish(client, userdata, mid):
    print("Message Published...")

def on_connect(client, userdata, flags, rc):
    client.subscribe(MQTT_TOPIC)
    client.publish(MQTT_TOPIC, MQTT_MSG)

def on_message(client, userdata, msg):
    print(msg.topic)
    print(msg.payload)
    payload = json.loads(msg.payload) # Use json.loads to convert string to json
    print(payload['BotID']) # Checks the value
    client.disconnect() # Got message then disconnect

# Initiate MQTT Client
mqttc = mqtt.Client()

# Register publish callback function
mqttc.on_publish = on_publish
mqttc.on_connect = on_connect
mqttc.on_message = on_message

# Connect with MQTT Broker
mqttc.connect(MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE_INTERVAL)

mqttc.loop_forever()
