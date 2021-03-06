
import time
import paho.mqtt.client as mqtt
import json
import requests


# Set this to the IP of your MQTT broker
mqtt_thingsboard = "thingsboard.cloud"
# Device auth token from device created in thingsboard
device_token = " ###### "
# Set MQTT topic.
topic = "v1/devices/me/telemetry"
# Coordinates for usage of API
lat = " ###### "
lon = " ###### "
# Yr.no API URL
APIurl = "https://api.met.no/weatherapi/locationforecast/2.0/compact.json?lat={}&lon={}".format(lat, lon)
# Header is required for using the API check therms and conditions 
headers = {
  'User-Agent': ' your email or other personal contact info'
}

if __name__ == '__main__':
    # Connect to Service domain MQTT
  try:
      mqttc = mqtt.Client()
      # Set username to device token
      mqttc.username_pw_set(username=device_token)
      mqttc.connect(mqtt_thingsboard, port=1883)
      mqttc.loop_start()
      # Main Loop
      while True:
          ts = round(time.time()*1000)
          # Get data from YR.no
          response = requests.request("GET", APIurl, headers=headers, data={})
          apiData = json.loads(response.text)
          trysilTemp = apiData['properties']['timeseries'][0]['data']['instant']['details']['air_temperature']
          trysilhPa = apiData['properties']['timeseries'][0]['data']['instant']['details']['air_pressure_at_sea_level']
          # Publish to thingsboard
          publishMsg = {"ts" : ts, "values" : {"trysilTemp" : str(trysilTemp), "trysilhPa" : str(trysilhPa)}}  
          mqttc.publish(topic, json.dumps(publishMsg)) # Publish to broker
          # Set time between data points, 300 = 5 min
          time.sleep(60)
  except Exception as e:
    print(e)
