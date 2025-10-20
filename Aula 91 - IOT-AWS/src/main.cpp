#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "internet.h"
#include "certificados.h"

const int mqtt_port = 8883;
const char *mqtt_id = "idDoEsp32";
const char *mqtt_sub = "senai134/comandos";
const char *mqtt_pub = "senai134/sensores";

WiFiClientSecure espClient;
PubSubClient mqtt(espClient);

void Callback(char *topic, byte *payLoad, unsigned int Length);
void ConectaMQTT();

void setup()
{
  Serial.begin(9600);
  conectaWiFi();

  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_CERT);
  espClient.setPrivateKey(AWS_KEY);

  mqtt.setServer(AWS_BROKER, mqtt_port);
  mqtt.setCallback(Callback);
}

void loop()
{
  checkWiFi();

  if (!mqtt.connected())
  {
    ConectaMQTT();
  }

  mqtt.loop();

  static unsigned long tempo_pub = 0;
  unsigned long agora = millis();

  if (agora - tempo_pub > 2000)
  {
    JsonDocument doc;

    doc["msm"] = "Oieee";

    String mensagem;
    serializeJson(doc, mensagem);
    mqtt.publish(mqtt_pub, mensagem.c_str());
    tempo_pub = agora;
  }
}

void Callback(char *topic, byte *payLoad, unsigned int Length)
{
  String msg((char *)payLoad, Length);
  Serial.print("..........................");
  Serial.printf("mensagem recebida (topico: [%s]): %s\n\r", topic, msg.c_str());
}

void ConectaMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Conectando ao AWS IoT core ...");
    if (mqtt.connect(mqtt_id))
    {
      mqtt.subscribe(mqtt_sub);

      Serial.println("Conectado.");
    }
    else
    {
      Serial.printf("falhou (%d). Tentando Novamente em 5s \n\r", mqtt.state());
      delay(5000);
    }
  }
}