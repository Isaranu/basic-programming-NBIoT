/* Library for cloud data sending */
#include "IoTtweetSocket.h"
#include "SocketIoClient.h"

/* Library for DS18B20 sensor */
#include <OneWire.h>
#include <DallasTemperature.h>

/* Library for WiFi configuration setting */
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>    

const char *ssid = "xxx";          /* WiFi SSID name*/
const char *passw = "xxx";         /* WiFi password */
String device_id = "device_6";     /* Set device name before connecting cloud */ 

const char *serverIP = "35.240.237.43";
int serverPort = 3000;

unsigned long prev_ms = 0;
const long cycleTime = 60; /* in second */

IoTtweetSocket socket;
SocketIoClient IoT;

/* Setup for DS18B20 sensor */
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float t;

void response(String payload, size_t length) {
  Serial.println("Receive response from server : " + payload);
  
  Serial.println("IoT send a device name to server");
  Serial.println("IoT name : " + device_id);

  String iotname = "\"" + device_id + "\"";
  char buff[255];
  unsigned int len = iotname.length();
  iotname.toCharArray(buff, len+1);
  
  IoT.emit("iot",buff);
}

void sendTemp(float t){
  
  String tempStr = String(t);
  char t_buff[255];
  unsigned int len = tempStr.length();
  tempStr.toCharArray(t_buff, len+1);
  
  IoT.emit("temp", t_buff);
}

void led(String payload, size_t length){
  switch (payload.toInt()) {
  case 1:
    digitalWrite(LED_BUILTIN, LOW);
    break;
  case 0:
    digitalWrite(LED_BUILTIN, HIGH);
    break;
  default:
    //..
    break; 
  }
}

void setup() {
  
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.autoConnect("IoT temp. monitor");
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  socket.begin(ssid,passw);

  sensors.begin();
      
  /* Set Event response from server */
  IoT.on("response", response);

  /* Set Event of turn ON, OFF LED */
  IoT.on("led", led);

  IoT.begin(serverIP, serverPort);
  prev_ms = millis();

}

void loop() {
  //..
  IoT.loop();

  unsigned long cur_ms = millis();
    if(cur_ms - prev_ms > cycleTime*1000){

      /* Send request to all slave of DS18B20 */
      Serial.println("Send request temperature to all DS18B20..");
      sensors.requestTemperatures();
      Serial.println("done !");
      t = sensors.getTempCByIndex(0);
      Serial.println("t = " + String(t));

      sendTemp(t);

      prev_ms = cur_ms;
    }
}
