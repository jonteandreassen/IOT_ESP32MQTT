# Smart landställe 1.0 Jonathan Andreassen

# Use case

### Bakgrund till projektet:

Min familj har en vinterstuga i fjällen, idén är att ta stugan från dåtiden och in i nutid/framtid genom att implementera "smarta" lösningar, för att kunna ha kontroll över energiförbrukning och klimatet i huset.

Tanken med det här projektet är att bygga grundstrukturen och en prototyp för att göra stugan "smart", och att i framtiden kunna bygga ut projektet med relevanta tekniska uppgraderingar. Exempelvis värmeslinga på taken för att smälta snö som kopplas till ett smart eluttag. 

### Systemskiss

![trysil1 0](https://user-images.githubusercontent.com/71496860/146353412-6750cb36-10e6-462f-97d0-63498579932e.jpg)


### Dashboard i Thingsboard
- Humidity mäts i %
- Snowfall mäts i milimeter
- Temperature mäts i Celcius

![dashboard](https://user-images.githubusercontent.com/71496860/146353433-65da705e-354b-4e1d-8d5f-600e4f47aee7.JPG)


### Varför:

Stugan ligger i Norge och i dagsläget så ställer vi värmen på elementen när vi kommer och åker därifrån, vilket inte är så smart i och med att det drar onödig energi, främst på sommarhalvåret.

Eftersom att stugan är en timmerstuga vill jag/vi därför ha koll på luftfuktigheten och eventuellt med hjälp av smarta uttag sätta på en extra värmefläkt vid behov.

#

### Effekt / nytta:

Med de stigande elpriserna så ger detta också ekonomiska incitament till att ta stugan i nutiden med smarta tekniska uppgraderingar för att kontrollera värme & elförbrukningen.

#

### Målgrupp:

Min familj, tanken är också att till sommaren om tid finns  även göra sommarstugan smart med några andra features. Vi har ett par solpaneler där och samlar också in regnvatten så jag tänker att jag skulle vilja bygga en "plantskola", där vi på våren sår från inne i snickarboden och bevattning görs med hjälp av sensorer och pumpar. Vattnet tas från vatteninsamlingarna och strömen för att driva sensorerna och devicen tas från solen.

# 

### Val av hård och mjukvara samt tjänster:

**VS Code med Arduino extension :** 

> Kombinationen valde jag för att jag ville testa att skriva Arduino C/C++ syntax i en ny miljö, både för att få intellisense men också för att jämföra mot Arduino IDE. Det  funkade väldigt bra och hade inga större problem. Det känns däremot som att den totala tiden att kompilera en fil är lite längre.

**Python:**

> Jag har inte skrivit python sedan starten av våran utbildning och tänkte att det var dags att göra igen.

**Thingsboard:**

> Jag använder mig av Thingsboard som IaaS (Infrastructure as a service), eftersom att jag använder deras verktyg för dashboarden samt lagring av data. Medans jag står för Applikation, Data, Runtime, Middleware och Operativsystem

> Det är ett smidigt verktyg att använda för att både lagra och visualisera data från sina sensorer. Jag hade inga problem alls att skicka in data från mina 3 sensorer, även fast alla 3 skickar in data på lite olika sätt även om alla är  över MQTT som protokol.

**Raspberry pi:**

> När jag väl är på plats i stugan kommer jag ladda över hela projektet och hosta de på raspberryn. Jag kommer använda mig av Raspbian som OS och sedan köra alla scripten.

> I framtiden så tänker jag nog övergå till någon form av containerlösning och styra raspberryn över SSH från Stockholm för en smidigare hantering.

**ESP32 microcontroller:**

> Jag valde den för att jag har den och tycker att den passar väldigt bra till ändamålet då jag behöver WiFi och en digital ingång på kortet för att koppla in min DHT11 sensor.

**DHT11 temperatur och luftfuktighetssensor:**

> Jag hade en sedan innan och den mäter med tillräckligt bra precision för ändamålet.

# 

### Typ av data:

Inomhus Data:

>Temperatur
>
>Luftfuktighet 
>
Utomhus Data:

>Temperatur
>
>Luftryck 
>
>Snöfall
>
### Hur data skall samlas in och hur detta uppnås:

Inomhus Data:

**ESP32 :**

Mäter temperatur och luftfuktighet som sparas in i varsin variabel och skickar sedan datan med MQTT till Thingsboards broker.

För att skicka från device till thingsboard så behöver man en access token och en url med i koden exempel:

```arduino
#include "ThingsBoard.h"
#include "DHT.h"
#include "WiFi.h"

#define TOKEN               " TOKEN from device in thingsboard"
#define THINGSBOARD_SERVER  "thingsboard.cloud"
```
#

Utomhus Data:

**API concole app:**

Gör request mot YR.no, där man anger longitud och latitud samt skickar med en header för att kunna hämta data. Jag hämtar temperatur och lufttryck från APIet och skickar sedan data med MQTT till Thingsboards broker.

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
#

**Snöfall concole app:**

Jag skapar upp en variabel snowfall som slumpar fram ett flyttal mellan 00.00 - 40.00 som symboliserar snöfall i millimeter. Detta skickas sedan via MQTT till Thingsboards broker.

För den här console appen använde jag mig av en annan import av MQTT som var rekommenderad av thingsboard och för att testa något nytt.

```python
from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo
import time
import random

device_token = ' ACCESS TOKEN '
# IP to broker
tbIP = 'thingsboard.cloud'
# Used for dashboard map, not needed since you can manually add
# it but I wanted to send it just for practice reasons
lat = " LATITUDE "
lon = " LONGITUDE "
```
#

### Källor och villkor för användning av YR.NO

> [https://developer.yr.no/doc/TermsOfService/](https://developer.yr.no/doc/TermsOfService/)
>

>[https://developer.yr.no/doc/GettingStarted/](https://developer.yr.no/doc/GettingStarted/)
>

#

### Hårdvara jag använt:

> ESP32
> 

> DHT11
> 

> Wifi router
> 

> Raspberry pi 3b+


> Wifi element, vi har två i stugan men i dagsläget är jag osäker på hur jag kan använda dem men jag skulle vilja kunna ha en symbios mellan
  elementen och temperaturvärdet som DHT11 sensorn läser in. 
> 
