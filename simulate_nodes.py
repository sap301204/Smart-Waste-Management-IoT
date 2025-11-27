#!/usr/bin/env python3
import time, json, random, argparse
import paho.mqtt.client as mqtt
from datetime import datetime
parser = argparse.ArgumentParser()
parser.add_argument("--host", default="localhost")
parser.add_argument("--port", type=int, default=1883)
parser.add_argument("--nodes", type=int, default=5)
parser.add_argument("--interval", type=float, default=30.0)
parser.add_argument("--depth", type=int, default=80)
args = parser.parse_args()
BROKER=args.host; PORT=args.port; NODES=args.nodes; INTERVAL=args.interval; BIN_DEPTH_CM=args.depth
client = mqtt.Client("simulator"); client.connect(BROKER, PORT, 60)
distances=[random.uniform(60,BIN_DEPTH_CM) for _ in range(NODES)]
directions=[-0.2*random.random() for _ in range(NODES)]
try:
    while True:
        for i in range(NODES):
            if random.random()<0.02:
                distances[i]=min(BIN_DEPTH_CM, distances[i]+random.uniform(10,30))
            else:
                distances[i]=max(2.0, distances[i]+directions[i]+random.uniform(-0.5,0.5))
            if distances[i]<5: distances[i]=5+random.random()*2
            temp=20+random.uniform(-3,4)
            hum=40+random.uniform(-10,10)
            gas=random.randint(100,400)
            payload={"node_id":f"bin-{i+1:03d}","timestamp":datetime.utcnow().isoformat(),"distance_cm":round(distances[i],2),"level_pct":round((1-(distances[i]/BIN_DEPTH_CM))*100,2),"temp_c":temp,"hum_pct":hum,"gas_raw":gas}
            topic=f"smartbin/bin-{i+1:03d}/data"
            client.publish(topic, json.dumps(payload))
            print(f"Published {topic} -> {payload['level_pct']}%")
        time.sleep(INTERVAL)
except KeyboardInterrupt:
    client.disconnect(); print('stopped')
