#include <Arduino.h>

#define pinLed 2
#define pinButton 0 

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
 bool buttonState = digitalRead(pinButton);
 Serial.println(buttonState);
 
 if(buttonState == 1){
    digitalWrite(2, LOW);
    delay(300);
   
    
  }else{
    digitalWrite(2, HIGH);
  }
}
