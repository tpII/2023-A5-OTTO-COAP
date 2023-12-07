
import paho.mqtt.client as mqtt
import time

start_ms = None

def on_connect(mqtt_client, obj, flags, rc):
    global start_ms
    mqtt_client.subscribe("movimientos")
    start_ms = time.time()
    mqtt_client.publish('otto','my test payload')

def on_message(mqtt_client, obj, msg):
    global start_ms
    end_ms = time.time()
    print("mqtt round trip time: ", (end_ms-start_ms)*1000, "ms")
    start_ms= time.time()

mqtt_client = mqtt.Client()
mqtt_client.username_pw_set('Otto', 'DefaultOtto')
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.connect("192.168.4.2")
mqtt_client.loop_forever()