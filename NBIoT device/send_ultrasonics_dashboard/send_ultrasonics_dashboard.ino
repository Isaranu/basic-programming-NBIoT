/*
Recommended use "Arduino MEGA 2560 for stable running
*/

#include "IoTtweetNBIoT.h"
#include <Ultrasonic.h>

String userid = "your-IoTtweet-account-ID";                         /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                      /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "AIS NB-IoT";              /*Your private tweet meassage to dashboard*/
String public_tweet = "Ultrasonics sensor";           /*Your public tweet message to dashboard*/

Ultrasonic ultrasonic(11,12); /* (Trig PIN,Echo PIN) */
int distances;

const long looptime = 5;                         /* Loop delay time in sec. */
unsigned long previousMillis = 0;

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
      
      /* Send Ultrasonic distances as every cycleTime */
      distances = ultrasonic.distanceRead();
      Serial.println("Ultrasonics distances = " + String(distances) + " cm."); 

      myNBiot.sendDashboard(userid,key,0,0,0,distances,private_tweet,public_tweet);
     
      previousMillis = currentMillis;
  
    }
     
}
