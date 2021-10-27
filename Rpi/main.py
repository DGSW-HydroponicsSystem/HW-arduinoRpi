import serial
import struct
import time
import re
import paho.mqtt.client as mqtt
import json

def on_connect(client, userdata, flags, rc):
    print('connected')
    client.subsribe('HydroponicsSystem/control')

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)
    if msg.topic == 'HydroponicsSystem/control':
        j = json.loads(msg.payload)
        if j['type'] == 'led':
            if j['cmd'] == 'on':
                ser.write(b'L')
            else:
                ser.write(b'l')
        elif j['type'] == 'waterPump':
            if j['cmd'] == 'on':
                ser.write(b'W')
            else:
                ser.write(b'w')

regex = b'^H(.{2})T(.{2})L(.{1})W(.{1})\n$'
def read_arduino(arduino):
    m = re.match(regex, arduino.readline())
    if m is not None:
        humidity = int.from_bytes(m[1], byteorder='little')
        temperature = int.from_bytes(m[2], byteorder='little')
        led = int.from_bytes(m[3], byteorder='little')
        waterpump = int.from_bytes(m[4], byteorder='little')
        
        return {
            'temperature' : temperature,
            'humidity' : humidity,
            'led' : led,
            'waterpump' : waterpump
        }
    else:
        return None

address = '13.209.41.37'
port = 1883

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(address, port)
client.loop_start()

try:
    global ser
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)

    ser.flushInput()
    while True:
        data = read_arduino(ser)
        if data is not None:
            print('{temperature}°C / {humidity}% / ledStat : {led} / waterpumpStat : {waterpump}'.format(**data))

            client.publish('HydroponicsSystem/stat', json.dumps(data))
        
        client.loop_read()
        time.sleep(1)

    ser.close()

except Exception as e:
    print(e)
    pass