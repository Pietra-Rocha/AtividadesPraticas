#include <Arduino.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#define pinbotaoboot 0

JsonDocument doc; 
Bounce botaoboot = Bounce();


void setup() 
{
  Serial.begin(9600);
  botaoboot.attach(pinbotaoboot, INPUT_PULLUP);
  random(analogRead(5));
}

void loop() 
{
  botaoboot.update();
  static int contagem = 0;

  if (botaoboot.fell())
  {
    contagem++;
  }
  
  const char* estados[] = {"ok", "alerta", "erro"};

  float temperatura = random(200, 350) / 10.0;

  int umidade = random(40, 80); 
  int opcoes = random(0, 3);

  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["estado"] = estados[opcoes];
  doc["clicks"] = contagem;

  unsigned long tempoAtual = millis();
  static unsigned long tempoInicial = 0;

  if (tempoAtual - tempoInicial > 5000)
  {
    serializeJson(doc, Serial);
    Serial.println(" ");
    tempoInicial = tempoAtual;
  }
  
  
}