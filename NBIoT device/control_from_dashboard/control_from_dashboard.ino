/*
Recommended use "Arduino MEGA 2560 for stable running
*/

#include "IoTtweetNBIoT.h"

#define relay_1 6   /*Connect relay to pin 6 of Arduino MEGA*/

String userid = "007867";       /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "twdvml1ge9qe";                   /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "Get Control to NB-IoT";              /*Your private tweet meassage to dashboard*/
String public_tweet = "Hello IoTtweet";           /*Your public tweet message to dashboard*/

const long looptime_task1 = 2;                    /* Task 1 time in sec. */

unsigned long previousMillis_task1 = 0;

IoTtweetNBIoT myNBiot;

void setup(){ 
  
  myNBiot.init();

  pinMode(relay_1, OUTPUT);
  digitalWrite(relay_1, LOW);
  
  previousMillis_task1 = millis();
  
}
void loop(){ 
  
  unsigned long currentMillis_task1 = millis();

  /* Task 1 : Get control status from dashboard */
  if (currentMillis_task1 - previousMillis_task1 >= looptime_task1*1000)
    { 
      
      /* Get control from IoTtweet dashboard */
      String sw1 = myNBiot.getControlbySwitch(userid, key, 1);
      Serial.println("sw1 = " + sw1);
      if(sw1 == "ON"){
        digitalWrite(relay_1, HIGH);
      }else{
        digitalWrite(relay_1, LOW);
      }
      
      previousMillis_task1 = currentMillis_task1;
    }
     
}
