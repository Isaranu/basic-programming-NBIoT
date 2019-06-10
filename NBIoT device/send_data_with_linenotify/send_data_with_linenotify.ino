#include "IoTtweetNBIoT.h"
#include "DHT.h"

/* Library for DS18B20 sensor */
#include <OneWire.h>
#include <DallasTemperature.h>

String userid = "your-iottweet-account-id";           /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-iottweet-device-key";              /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                     /*Your sending data variable.*/
String private_tweet = "AIS NB-IoT with Line Notify"; /*Your private tweet meassage to dashboard*/
String public_tweet = "Hello IoTtweet";               /*Your public tweet message to dashboard*/

/* Setup for DHT22 sensor */
#define DHTPIN 7 /* connect to Arduino UNO pin D7 */
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/* Setup for DS18B20 sensor */
#define ONE_WIRE_BUS 3  //Connect DS18B20 to port 3 on UNO.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float t_dht22, h_dht22;
float t_ds18b20;

const long looptime_task1 = 5;                        /* Task 1 time in sec., Example in 5 second */
const long looptime_task2 = 1*60;                    /* Task 2 time in sec., Example in 10 mins */

unsigned long previousMillis_task1 = 0;
unsigned long previousMillis_task2 = 0;

/* LINE notify token parameter */
String LINE_TOKEN = "your-line-token";

IoTtweetNBIoT myNBiot;

void setup(){ 
  
  dht.begin();
  myNBiot.init();
  
  previousMillis_task1 = millis();
  previousMillis_task2 = millis();
  
}
void loop(){ 
  
  unsigned long currentMillis_task1 = millis();
  unsigned long currentMillis_task2 = millis();

  /* Task 1 : Send data to IoTtweet dashboard */
  if (currentMillis_task1 - previousMillis_task1 >= looptime_task1*1000)
    { 
      
      /* - DHT sensor reading - */
      t_dht22 = dht.readTemperature();
      h_dht22 = dht.readHumidity();

      if (isnan(t_dht22) || isnan(h_dht22)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }

      /* Send request to all slave of DS18B20 */
      Serial.println("Send request temperature to all DS18B20..");
      sensors.requestTemperatures();
      Serial.println("done !");
      t_ds18b20 = sensors.getTempCByIndex(0);
      Serial.println("t = " + String(t_ds18b20));

      Serial.println("----------- Read DHT sensor ------------");
      Serial.println("[DHT22] Temperature is " + String(t_dht22) + " celcuis");
      Serial.println("[DHT22] Humidity is " + String(h_dht22) + " %RH");
      Serial.println("----------------------------------------");
      Serial.println("----------- Read DS18B20 sensor ------------");
      Serial.println("[DS18B20] Temperature is " + String(t_ds18b20) + " celcuis");
      Serial.println("----------------------------------------");

      myNBiot.sendDashboard(userid,key,t_dht22,h_dht22,t_ds18b20,0,private_tweet,public_tweet);
      previousMillis_task1 = currentMillis_task1;
      
     }

  /* Task 2 : Send LINE notify */
  if (currentMillis_task2 - previousMillis_task2 >= looptime_task2*1000)
    {
      String line_msg = "[DHT22] temperature = ";
             line_msg += String(t_dht22);
             line_msg += "| Humidity = ";
             line_msg += String(h_dht22);
             line_msg += "| Temp DS18b20 = ";
             line_msg += String(t_ds18b20);
      
      myNBiot.pushLineNotify(LINE_TOKEN, line_msg);
      previousMillis_task2 = currentMillis_task2;
    }
     
}
