import paho.mqtt.client as mqtt
import json

# This is the Subscriber

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe("payload")

def on_message(client, userdata, msg):
   jsondata = msg.payload.decode()
   jsontopython = json.loads(jsondata)
   print(jsontopython)
   #client.disconnect()
    
client = mqtt.Client()
client.connect("127.0.0.1",1883,60)

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()