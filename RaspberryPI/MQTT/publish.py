import json


def send(client, topic, data):
    client.publish(topic, json.dumps(data))


