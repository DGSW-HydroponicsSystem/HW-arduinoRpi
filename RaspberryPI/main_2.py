import json
import serial
import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):
    print('connected')
    client.subsribe('HydroponicsSystem/control')

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)
    if msg.topic == 'HydroponicsSystem/control':
        data = json.loads(msg.payload)
        if data['type'] == 'led':
            if data['cmd'] == 'on':
                ser2.write(b'L')
            elif data['cmd'] == 'off':
                ser2.write(b'l')
        elif data['type'] == 'waterPump':
            if data['cmd'] == 'on':
                ser2.write(b'W')
            elif data['cmd'] == 'off':
                ser2.write(b'w')
        elif data['type'] == 'fan':
            if data['cmd'] == 'on':
                ser2.write(b'F')
            elif data['cmd'] == 'off':
                ser2.write(b'f')

def read_arduino():
    msg = ser1.readline().decode('utf-8').rstrip()
    if msg is not None:
        return msg
    else:
        msg = ser2.readline().decode('utf-8').rstrip()
        if msg is not None:
            return msg
        else:
            return None

def split_string(msg):
    index = msg.split('/')
    for x in range(index):
        index[x] = int(index[x])

    if index[0] == 0:
        temperature1 = index[1]
        temperature2 = index[2]
        humidity1 = index[3]
        humidity2 = index[4]
        sunlight = index[5]
        watertemp = index[6]
        waterlevel = index[7]
        waterph = index[8]

        return {
            'Key' : 0,
            'temperature1' : temperature1,
            'temperature2' : temperature2,
            'humidity1' : humidity1,
            'humidity2' : humidity2,
            'sunlight' : sunlight,
            'watertemp' : watertemp,
            'waterlevel' : waterlevel,
            'waterph' : waterph
        }
    elif index[0] == 1:
        waterpumpstat = index[1]
        ledstat = index[2]
        fanstat = index[3]

        return {
            'Key' : 1,
            'waterpump' : waterpumpstat,
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
ser1 = serial.Serial('tty/ACM0', 9600, timeout=1)
ser1.flushInput()
ser2 = serial.Serial('tty/ACM1', 9600, timeout=1)
ser2.flushInput()

try:
    while True:
        data = read_arduino()
        if data is not None:
            dict_data = split_string(data)
            client.publish('HydroponicsSystem/stat', json.dumps(dict_data))

        client.loop_read()
        ser1.flushInput()
        ser2.flushInput()

except Exception as E:
    print(E)
    pass

