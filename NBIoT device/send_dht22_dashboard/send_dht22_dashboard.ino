#include "IoTtweetNBIoT.h"
#include "DHT.h"

String userid = "your-IoTtweet-account-ID";                         /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                      /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "AIS NB-IoT";              /*Your private tweet meassage to dashboard*/
String public_tweet = "DHT22 sensor";           /*Your public tweet message to dashboard*/

/* Setup for DHT22 sensor */
#define DHTPIN 7 /* connect to Arduino UNO pin D7 */
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const long looptime = 20;                         /* Loop delay time in sec. */
unsigned long previousMillis = 0;

long cnt = 0;

float t_dht22, h_dht22;

IoTtweetNBIoT myNBiot;

void setup()
{ 
  dht.begin();
  myNBiot.init();
  previousMillis = millis();

}
void loop()
{ 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= looptime*1000)
    {
      cnt++;     
      
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
     
      previousMillis = currentMillis;
  
    }
     
}
