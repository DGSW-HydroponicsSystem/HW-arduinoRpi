import paho.mqtt.client as mqtt
import serial
import MQTT
import arduino

# mqtt
address = '127.0.0.1'
port = 1883
topic = 'HydroponicsSystem/stat'

client = mqtt.Client()
client.on_connect = MQTT.subscribe.on_connect
client.on_message = MQTT.subscribe.on_message

client.connect(address, port)
client.loop_start()

# arduino serial
ser = serial.Serial('tty/', 9600, timeout=1)
ser.flushInput()
#
try:
    while True:
        data = arduino.s.read_arduino(ser)
        if data is not None:
            j_data = arduino.s.split_string(data)
            MQTT.publish.send(client, topic, j_data)

        client.loop_read()
        ser.flushInput()

except Exception as E:
    print(E)
