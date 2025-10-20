#include <Arduino.h>
#define pinM0Dir 14
#define pinM0Esq 13
#define pinM1Dir 10
#define pinM1Esq 46
#define pinM2Dir 3
#define pinM2Esq 18
#define pinM3Dir 12
#define pinM3Esq 11
#define canal0 0 //MOTOR 1
#define frequenciaPWM 20000
#define resolucaoPWM 8
  

void setup() 
{
 Serial.begin(9600);

 pinMode(pinM0Dir , OUTPUT);
 pinMode(pinM0Dir , OUTPUT);
 pinMode(pinM1Dir , OUTPUT);
 pinMode(pinM1Esq , OUTPUT);
 pinMode(pinM2Dir , OUTPUT);
 pinMode(pinM2Esq , OUTPUT);
 pinMode(pinM3Dir , OUTPUT);
 pinMode(pinM3Esq , OUTPUT);

 ledcSetup(canal0, frequenciaPWM, resolucaoPWM);
 ledcAttachPin(pinM0Dir, canal0);
}

void loop() 
{
 for (int i = 0; i <= 250; i = i + 10)
 {
  ledcWrite(canal0, i);
  Serial.printf("velocidade em %d \n", i);
  delay(3000);
 }
}