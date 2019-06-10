#include "IoTtweetNBIoT.h"

/* Library for DS18B20 sensor */
#include <OneWire.h>
#include <DallasTemperature.h>

String userid = "your-IoTtweet-account-ID";                         /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                      /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "AIS NB-IoT";              /*Your private tweet meassage to dashboard*/
String public_tweet = "DS18b20 sensor";           /*Your public tweet message to dashboard*/

/* Setup for DS18B20 sensor */
#define ONE_WIRE_BUS 3  //Connect DS18B20 to port 3 on UNO.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const long looptime = 20;                         /* Loop delay time in sec. */
unsigned long previousMillis = 0;

long cnt = 0;

float t_ds18b20;

IoTtweetNBIoT myNBiot;

void setup()
{ 
  myNBiot.init();
  previousMillis = millis();

}
void loop()
{ 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= looptime*1000)
    {
      cnt++;     

      /* Send request to all slave of DS18B20 */
      Serial.println("Send request temperature to all DS18B20..");
      sensors.requestTemperatures();
      Serial.println("done !");
      t_ds18b20 = sensors.getTempCByIndex(0);
      Serial.println("t = " + String(t_ds18b20));

      Serial.println("----------- Read DS18B20 sensor ------------");
      Serial.println("[DS18B20] Temperature is " + String(t_ds18b20) + " celcuis");
      Serial.println("----------------------------------------");

      myNBiot.sendDashboard(userid,key,0,0,t_ds18b20,0,private_tweet,public_tweet);
     
      previousMillis = currentMillis;
  
    }
     
}
