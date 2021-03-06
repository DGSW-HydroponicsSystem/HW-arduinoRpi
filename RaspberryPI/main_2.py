import json
import serial
import paho.mqtt.client as mqtt
import time


def on_connect(client, userdata, flags, rc):
    print('connected')
    client.subscribe('HydroponicsSystem/control')

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)
    if msg.topic == 'HydroponicsSystem/control':
        data = json.loads(msg.payload)
        if data['type'] == 'led':
            if data['cmd'] == 'on':
                ser2.write(b'L')
            elif data['cmd'] == 'off':
                ser2.write(b'l')
        elif data['type'] == 'waterPump1':
            if data['cmd'] == 'on':
                ser2.write(b'W')
            elif data['cmd'] == 'off':
                ser2.write(b'w')
        elif data['type'] == 'waterPump2':
            if data['cmd'] == 'on':
                ser2.write(b'P')
            elif data['cmd'] == 'off':
                ser2.write(b'p')
        elif data['type'] == 'fan':
            if data['cmd'] == 'on':
                ser2.write(b'F')
            elif data['cmd'] == 'off':
                ser2.write(b'f')

def read_arduino():
    msg1 = ser1.readline()
    msg2 = ser2.readline()
    if str(msg2) != "b''":
        return msg2
    elif str(msg1) != "b''":
        return msg1
    else:
        return None

def split_string(msg):
    index = list(bytes(msg))

    if index[0] == 0:
        temperature1 = index[1]
        temperature2 = index[2]
        humidity1 = index[3]
        humidity2 = index[4]
        sunlight1 = index[5]
        sunlight2 = index[6]
        watertemp1 = index[7]
        watertemp2 = index[8]
        waterlevel = index[9]
        waterph = index[10]/100

        return {
            'Key' : 0,
            'temperature1' : temperature1,
            'temperature2' : temperature2,
            'humidity1' : humidity1,
            'humidity2' : humidity2,
            'sunlight1' : sunlight1,
            'sunlight2' : sunlight2,
            'watertemp1' : watertemp1,
            'watertemp2' : watertemp2,
            'waterlevel' : waterlevel,
            'waterph' : waterph
        }
    elif index[0] == 1:
        waterpumpstat1 = index[1]
        waterpumpstat2 = index[2]
        ledstat = index[3]
        fanstat = index[4]

        return {
            'Key' : 1,
            'waterpumpstat1' : waterpumpstat1,
            'waterpumpstat2' : waterpumpstat2,
            'ledstat' : ledstat,
            'fanstat' : fanstat
        }
    else:
        return None

# mqtt
address = '3.34.123.190'
port = 1883

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(address, port)
client.loop_start()

# serial
ser1 = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser2 = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

try:
    while True:
        data = read_arduino()
        if data is not None:
            dict_data = split_string(data)
            if dict_data['Key'] == 0:
                client.publish('HydroponicsSystem/stat', json.dumps(dict_data))
            elif dict_data['Key'] == 1:
                client.publish('HydroponicsSystem/module', json.dumps(dict_data))

        client.loop_read()
        ser1.flushInput()
        ser2.flushInput()
        time.sleep(1)

except Exception as E:
    print(E)
    ser1.close()
    ser2.close()
    pass

