from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo
import time
import random




device_token = 'AkPgmtsFpTl1cREzMGEr'
tbIP = 'thingsboard.cloud'

lat = "61.316900"
lon = "12.259839"


if __name__ == '__main__':
    
    try:
        while True:
            # Generate a  random snowfall float number in mm
            snowfall = round(random.uniform(00.00, 40.00), 2)

            #timestamp
            ts = round(time.time()*1000)

            # setup client with device token and ip to thingsboard cloud
            client = TBDeviceMqttClient(tbIP, device_token)
            
            # Connect to ThingsBoard
            client.connect()
            
                    # Sending telemetry without checking the delivery status
                    #client.send_telemetry(telemetry) 

            # message to send 
            telemetry = {"ts" : ts, "values" : {"snowfall" : str(snowfall), "latitude" : str(lat), "longitude" : str(lon)}}
            
            # Sending telemetry and checking the delivery status (QoS = 1 by default)
            result = client.send_telemetry(telemetry)
            
            # get is a blocking call that awaits delivery status  
            success = result.get() == TBPublishInfo.TB_ERR_SUCCESS
            # print out the result from publishing as a boolean
            print(success)

            # Disconnect from ThingsBoard
            client.disconnect()
            
            #10 seconds delay
            time.sleep(60) 
    except Exception as e:
        print(e)