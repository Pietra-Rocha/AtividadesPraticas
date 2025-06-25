#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "internet.h"
#include "certificados.h"


const int mqtt_port = 8883;
const char *mqtt_id = "esppietramesa04";
const char *mqtt_sub = "senai134/mesa04";
const char *mqtt_pub = "senai134/mesa04";
const char *mqttTopic = "senai134/alerta";

WiFiClientSecure espClient;
PubSubClient mqtt(espClient);


void ConectaMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Conectando ao AWS IoT core ...");
    if (mqtt.connect(mqtt_id))
    {
      Serial.println("MQTT conectado");
      mqtt.subscribe(mqtt_sub);
    }
    else
    {
      Serial.print("Erro: ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void enviarLembrete(const char *texto)
{
  JsonDocument doc;
  doc["lembrete"] = texto;

  String mensagem;
  serializeJson(doc, mensagem);
  mqtt.publish(mqtt_pub, mensagem.c_str());
}

void Callback(char *topic, byte *payLoad, unsigned int Length)
{
  String msg((char *)payLoad, Length);
  Serial.print("..........................");
  Serial.printf("mensagem recebida (topico: [%s]): %s\n\r", topic, msg.c_str());
}

void callbackalerta(char* topic, byte* payload, unsigned int length) {

  String mensagemRecebida((char *)payload, length);
  Serial.print("..........................");
  Serial.printf("📩 Mensagem recebida (topico: [%s]): %s\n\r", topic, mensagemRecebida.c_str());


  // Interpretar JSON
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagemRecebida);

  if (erro) {
    Serial.print("❌ Erro ao ler JSON: ");
    Serial.println(erro.c_str());
    return;
  }

  // Acessar dados do JSON
  const char* tipo = doc["tipo"];
  const char* msg = doc["mensagem"];
  const char* origem = doc["dispositivo"];

  if (strcmp(tipo, "alerta") == 0) {
    Serial.println("⚠️ ALERTA recebido!");
    Serial.print("🔔 Mensagem: ");
    Serial.println(msg);
    Serial.print("📟 Dispositivo: ");
    Serial.println(origem);

    // Aqui você pode ativar um LED, buzzer, display, etc.
  }
}

void conectarMQTTalerta() {
  while (!mqtt.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqtt.connect("espReceptor")) {
      Serial.println("✅ Conectado!");
      mqtt.subscribe(mqttTopic);
      Serial.print("📡 Inscrito no tópico: ");
      Serial.println(mqttTopic);
    } else {
      Serial.print("Erro: ");
      Serial.print(mqtt.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
    }
  }
}


void setup()
{
  Serial.begin(9600);

  conectaWiFi();


  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_CERT);
  espClient.setPrivateKey(AWS_KEY);

  mqtt.setServer(AWS_BROKER, mqtt_port);
  mqtt.setCallback(Callback);
  mqtt.setCallback(callbackalerta);
  enviarLembrete("Beber agua!");

  
}

void loop()
{
  checkWiFi();

  if (!mqtt.connected())
    ConectaMQTT();


  if (!mqtt.connected())
    conectarMQTTalerta();


  mqtt.loop();

}