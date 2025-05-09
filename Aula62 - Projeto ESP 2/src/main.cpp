#include <Arduino.h>
#include <ArduinoJson.h>
#include <Bounce2.h>

#define pinBotaoFrente 0
#define pinBotaoDireita 12
#define pinBotaoRe 14
#define pinBotaoEsquerda 27

JsonDocument doc;

uint8_t endSlave[] = {0x3C, 0x61, 0x05, 0x1C, 0x90, 0xA6}; // endereco do dispositivo a ser conectado

Bounce BotaoFrente = Bounce();
Bounce BotaoDireita = Bounce();
Bounce BotaoRe = Bounce();
Bounce BotaoEsquerda = Bounce();


void setup()
{
  Serial.begin(9600);

  BotaoFrente.attach(pinBotaoFrente, INPUT_PULLUP);
  BotaoDireita.attach(pinBotaoDireita, INPUT_PULLUP);
  BotaoRe.attach(pinBotaoRe, INPUT_PULLUP);
  BotaoEsquerda.attach(pinBotaoEsquerda, INPUT_PULLUP);
 
}

void loop()
{
  BotaoFrente.update();
  BotaoDireita.update();
  BotaoRe.update();
  BotaoEsquerda.update();

  unsigned long tempoAtual = millis();
  static unsigned long tempoInicial = 0;


  if(BotaoFrente.changed())
  {
    doc["frente"] = BotaoFrente.read();
  }
  if(BotaoDireita.changed())
  {
    doc["direita"] = BotaoDireita.read();
  }

  serializeJson(doc, Serial);
  delay(1000);
}
