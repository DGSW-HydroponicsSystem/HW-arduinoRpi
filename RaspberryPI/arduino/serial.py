import json


def read_arduino(arduino):
    data = arduino.readline().decode('utf-8').rstrip()
    if data is not None:
        return data
    else:
        return None


# 온도 2, 습도 2, 일조량 1, 수온 1, 물높이 1, 산성도 1
# 1층 온도/2층 온도/1층 습도/2층 습도/일조량/수온/물높이/산성도
def split_string(data):
    index = data.split('/')
    for x in range(index):
        index[x] = int(index[x])

    temperature1 = index[0]
    temperature2 = index[1]
    humidity1 = index[2]
    humidity2 = index[3]
    sunlight = index[4]
    watertemp = index[5]
    waterlevel = index[6]
    waterph = index[7]

    return {
            'temperature1' : temperature1,
            'temperature2' : temperature2,
            'humidity1' : humidity1,
            'humidity2' : humidity2,
            'sunlight' : sunlight,
            'watertemp' : watertemp,
            'waterlevel' : waterlevel,
            'waterph' : waterph
        }


# msg format - char?
def control_arduino(arduino, msg):
    arduino.write(msg)

