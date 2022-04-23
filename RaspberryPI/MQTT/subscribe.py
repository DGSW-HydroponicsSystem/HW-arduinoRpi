import json

global flag


def on_connect(client, userdata, flags, rc):
    print('connected')
    client.subscribe('HydroponicsSystem/control')


# 제어 -
def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)
    if msg.topic == '':
        data = json.loads(msg.payload)
        if data['type'] == 'led':
            if data['cmd'] == 'on':
                flag = 'L'
            elif data['cmd'] == 'off':
                flag = 'l'
        elif data['type'] == 'waterPump':
            if data['cmd'] == 'on':
                flag = 'W'
            elif data['cmd'] == 'off':
                flag = 'w'
        elif data['type'] == 'fan':
            if data['cmd'] == 'on':
                flag = 'F'
            elif data['cmd'] == 'off':
                flag = 'f'
