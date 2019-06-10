#include <Ultrasonic.h>

Ultrasonic ultrasonic(11,12); /* (Trig PIN,Echo PIN) */
int distances;

void setup(){

  Serial.begin(9600);
  
}

void loop() {

   /* Send Ultrasonic distances as every cycleTime */
   distances = ultrasonic.distanceRead();
   Serial.println(distances); 

   delay(50);
}
