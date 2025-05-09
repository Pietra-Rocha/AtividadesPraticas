#include <Arduino.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include <BluetoothSerial.h>

#define pinBotaoFrente 13
#define pinBotaoDireita 12
#define pinBotaoRe 14
#define pinBotaoEsquerda 27

JsonDocument doc;
BluetoothSerial BT;

uint8_t endSlave[] = {0x08, 0x3A, 0xF2, 0x8E, 0xDE, 0xDA}; // endereco do dispositivo a ser conectado

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

  random(analogRead(5));

  //!                       ↓ indica que eh master
  if (BT.begin("ESPmaster", true)) // inicia a conecxao bluetooth
  {
    Serial.println("Bluetooth iniciado c sucesso!");
  }
  else
  {
    Serial.println("Erro na inicializacao do Bluetooth!");
  }

  if (BT.connect(endSlave)) // conecta ao dispositivo
  {
    Serial.println("conectado com sucesso!");
  }
  else
  {
    Serial.println("nao foi possivel conectar ao dispositivo");
  }
}

void loop()
{
  BotaoFrente.update();
  BotaoDireita.update();
  BotaoRe.update();
  BotaoEsquerda.update();

  unsigned long tempoAtual = millis();
  static unsigned long tempoInicial = 0;
  bool atualizacao = false;

  // tratamento de botoes

  // BOTAO 1 PRESSIONADO
  if (BotaoFrente.changed())
  {
    doc["frente"] = BotaoFrente.read();
    Serial.println(BotaoFrente.fell());
    atualizacao = true;
  }

  // BOTAO 2 PRESSIONADO
  if (BotaoDireita.changed())
  {
    doc["direita"] = BotaoDireita.read();
    Serial.println(BotaoDireita.fell());
    atualizacao = true;
  }

  // BOTAO 3 PRESSIONADO
  if (BotaoRe.changed())
  {
    doc["re"] = BotaoRe.read();
    Serial.println(BotaoRe.fell());
    atualizacao = true;
  }

  // BOTAO 4 PRESSIONADO
  if (BotaoEsquerda.changed())
  {
    doc["esquerda"] = BotaoEsquerda.read();
    Serial.println(BotaoEsquerda.fell());
    atualizacao = true;
  }

  // BOTAO Analogico

  //****** FIM DO TRATAMENTO DOS BOTOES ******

  // atualizacao de envio
  if (atualizacao == 1)
  {
    String dados;
    serializeJson(doc, dados);
    Serial.println(" ");
    BT.print(dados + '\n');
  }
}