/*
Recommended use "Arduino MEGA 2560 for stable running
*/

#include "AIS_NB_BC95.h"

/* Library for DS18B20 sensor */
#include <OneWire.h>
#include <DallasTemperature.h>

/* Setup for DS18B20 sensor */
#define ONE_WIRE_BUS 3 // Connect DS18B20 to port 3 on UNO.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float t;

String apnName = "devkit.nb";
String serverIP = "8.8.8.8";  /* Testing IP server */
String serverPort = "5683";   /* Default port UDP : 5683 */
String udpData = "HelloWorld";

AIS_NB_BC95 AISnb;

const long interval = 5;  /* set delay loop second */
unsigned long previousMillis = 0;

void setup(){
   
  AISnb.debug = true;
  
  Serial.begin(9600);
 
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();  
  delay(1000);
  
  pingRESP pingR = AISnb.pingIP(serverIP);
  previousMillis = millis();

}

void loop(){
   
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval*1000)
    {
      
      /* Send request to all slave of DS18B20 */
      Serial.println("Send request temperature to all DS18B20..");
      sensors.requestTemperatures();
      Serial.println("done !");
      t = sensors.getTempCByIndex(0);
      Serial.println("t = " + String(t));

      previousMillis = currentMillis;
    }
  
    UDPReceive resp = AISnb.waitResponse();
}
