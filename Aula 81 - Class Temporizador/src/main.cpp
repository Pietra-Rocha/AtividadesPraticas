#include <Arduino.h>
#include "temporizador.h"
#include "internet.h"
#include <PubSubClient.h>
#include <WiFi.h>

//* -------ESTANCIAS DE OBJETOS
WiFiClient espClient;
PubSubClient client(espClient);
temporizador temp(1000);


//* ---------------------------------------------
//*     VARIAVEIS
//* ---------------------------------------------
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_pietra";
const char *mqtt_topic_sub = "senai134/mesa02/esppietra/subs";
const char *mqtt_topic_pub = "senai134/mesa02/esppietra/public";


void setup() {
  Serial.begin(9600);
  conectaWiFi();
  temp.iniciar();

}

void loop() {
  checkWiFi();
  client.loop();

  if (temp.estaPronto()) {
    client.publish(mqtt_topic_pub, "Mensagem enviada!");
    Serial.print("Mensagem enviada!");
    Serial.println(temp.estaPronto());
    temp.reiniciar();
  }
}