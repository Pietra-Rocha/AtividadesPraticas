#include <Arduino.h>
#include <ArduinoJson.h>

JsonDocument doc;

void setup()
{
 Serial.begin(9600);
}

void loop() 
{
  if (Serial.available())
  {
    String msm = Serial.readStringUntil('\n');
    msm.trim();

    deserializeJson(doc, msm);

    float temperatura = doc["temperatura"];
    int umidade = doc["umidade"];
    String estado = doc["estado"];

    Serial.println(" ");
    Serial.printf("a temperatura eh: %.1f \n\r", temperatura);
    Serial.printf("a umidade esta em  %d% \n\r", umidade);
    Serial.printf("o estado esta: %s \n\r", estado);

  }
  
}