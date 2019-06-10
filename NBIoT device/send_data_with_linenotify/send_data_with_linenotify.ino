#include "IoTtweetNBIoT.h"
#include "DHT.h"

String userid = "your-iottweet-account-id";           /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-iottweet-device-key";              /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                     /*Your sending data variable.*/
String private_tweet = "AIS NB-IoT with Line Notify";              /*Your private tweet meassage to dashboard*/
String public_tweet = "Hello IoTtweet";               /*Your public tweet message to dashboard*/

/* Setup for DHT22 sensor */
#define DHTPIN 7 /* connect to Arduino UNO pin D7 */
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float t_dht22, h_dht22;

const long looptime_task1 = 5;                        /* Task 1 time in sec., Example in 5 second */
const long looptime_task2 = 10*60;                    /* Task 2 time in sec., Example in 10 mins */

unsigned long previousMillis_task1 = 0;
unsigned long previousMillis_task2 = 0;

/* LINE notify token parameter */
String LINE_TOKEN = "your-line-notify-token";

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

      Serial.println("----------- Read DHT sensor ------------");
      Serial.println("[DHT22] Temperature is " + String(t_dht22) + " celcuis");
      Serial.println("[DHT22] Humidity is " + String(h_dht22) + " %RH");
      Serial.println("----------------------------------------");

      myNBiot.sendDashboard(userid,key,t_dht22,h_dht22,0,0,private_tweet,public_tweet);
      previousMillis_task1 = currentMillis_task1;
     }

  /* Task 2 : Send LINE notify */
  if (currentMillis_task2 - previousMillis_task2 >= looptime_task2*1000)
    {
      String line_msg = "[DHT22] temperature = ";
             line_msg += String(t_dht22);
             line_msg += "| Humidity = ";
             line_msg += String(h_dht22);
      
      myNBiot.pushLineNotify(LINE_TOKEN, line_msg);
      previousMillis_task2 = currentMillis_task2;
    }
     
}
