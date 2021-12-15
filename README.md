## Smart landställe 1.0 Jonathan Andreassen

## Use case

## Bakgrund till projektet:

Min familj har en vinterstuga i fjällen, idén är att göra stugan smart och tanken med det här projektet är att bygga grundstrukturen för att göra stugan smart, och förhoppningen är att i framtiden kunna bygga ut projektet med relevanta tekniska uppgraderingar. Exempelvis värmeslinga på taken för att smälta snö som kopplas till ett smart elluttag. 

Stugan ligger i Norge och i dagsläget så ställer vi temperaturen på elementen när vi kommer och åker därifrån, vilket inte är så smart i och med att det drar onödig ström, främst sommarhalvåret.

![trysil1 0](https://user-images.githubusercontent.com/71496860/146207242-04b3fa62-d80d-4e95-80a4-76cee50e18ce.jpg)

![dashboard](https://user-images.githubusercontent.com/71496860/146207331-8a24a6df-fafc-4143-9fc1-5b99bfd3689f.JPG)

# Varför:
Jag / familjen vill ha en dashboard / interface som visar temperatur och luftfuktighet inne i stugan samt en väderprognos. Så att vi lätt kan korrigera värmen i stugan via element eller värmefläkt 

Eftersom att stugan är en timmerstuga vill jag/vi därför ha koll på luftfuktigheten och eventuellt med hjälp av smarta uttag sätta på en extra värmefläkt vid behov.

Val av kodspråk och tjänster:

**Arduino:**

**Python:**

**Thingsboard:**

# Effekt / nytta:
  Med de stigande elpriserna så ger detta också ekonomiska incitament till att ta stugan i nutiden med smarta tekniska uppgraderingar för att kontrollera värme & elförbrukningen.

# Målgrupp:
  Min familj, tanken är också att till sommaren om tid finns  även göra sommarstugan smart med några andra features. Vi har ett par solpaneler där och samlar också in regnvatten så jag tänker att jag skulle vilja bygga en "plantskola", där vi på våren sår från inne i snickarboden och bevattning görs med hjälp av sensorer och pumpar. Vattnet tas från vatteninsamlingarna och strömen för att driva sensorerna och devicen tas från solen.

## Typ av data:

  ## Inomhusdata:

  Temperatur

  Luftfuktighet 

  ## Utomhusdata:

  Temperatur

  Luftryck 

  Snöfall

# Hur data skall samlas in och hur detta uppnås:

## Inomhusdata:

**DHT11 sensor** (temperatur och luftfuktighet) kopplad till en ESP32 som sparar in i varsin variabel och skickar sedan datan med MQTT till Thingsboards broker.

För att skicka från device till thingsboard så behöver man en access token och en url med i koden exempel:

```arduino
#include "ThingsBoard.h"
#include "DHT.h"
#include "WiFi.h"

#define TOKEN               " TOKEN from device in thingsboard"
#define THINGSBOARD_SERVER  "thingsboard.cloud"
```

## Utomhusdata:

**API concole app** som gör request mot YR.no, där man anger longitude och latitud samt skickar med en header för att kunna hämta data. Jag hämtar temperatur och lufttryck från APIet och skickar sedan datan med MQTT till Thingsboards broker.

```python
import time
import paho.mqtt.client as mqtt
import json
import requests

# IP of your MQTT broker
mqtt_thingsboard = "thingsboard.cloud"
# Device auth token from device created in thingsboard
device_token = " "
# Set MQTT topic this is required and standard.
topic = "v1/devices/me/telemetry"
# Your coordinates
lat = " YOUR LATITUDE"
lon = " YOUR LONGITUDE "
# Yr.no API URL
APIurl = "https://api.met.no/weatherapi/locationforecast/2.0/compact.json?lat={}&lon={}".format(lat, lon)
# Header is required for using the API
headers = {
  'User-Agent': ' your email for exampel '
}
```

**Snöfall concole app** där jag skapar en variabel snowfall som slumpar fram ett floattal mellan 00.00 - 40.00 som symboliserar snöfall i milimeter. Detta skickas sedan via MQTT till Thingsboards broker.

För den här console appen använde jag mig av en annan import av MQTT som var rekommenderad av thingsboard och för att testa något nytt.

```python
from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo
import time
import random

device_token = ' ACCESS TOKEN '
# IP to broker
tbIP = 'thingsboard.cloud'
# Used for dashboard map, not needed since you can manually add it but I wanted to send it just for practice reasons
lat = " LATITUDE "
lon = " LONGITUDE "
```

# Källor och villkor för användning av YR.NO

[https://developer.yr.no/doc/TermsOfService/](https://developer.yr.no/doc/TermsOfService/)

[https://developer.yr.no/doc/GettingStarted/](https://developer.yr.no/doc/GettingStarted/)

[https://thingsboard.io/docs/reference/python-client-sdk/](https://thingsboard.io/docs/reference/python-client-sdk/)

### Hårdvara jag använt:

- ESP32
- DHT11
- Wifi router
