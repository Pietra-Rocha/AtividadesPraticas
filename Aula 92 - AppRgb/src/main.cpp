#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "internet.h"
#include "certificados.h"
#include <Adafruit_NeoPixel.h>

//* ===== CONFIG MQTT =====
const int mqtt_port = 8883;
const char *mqtt_id = "esppietramesa01";
const char *mqtt_sub = "teste/led";
const char *mqtt_pub = "teste/led";
const char *mqttTopic = "teste/led";

WiFiClientSecure espClient;
PubSubClient mqtt(espClient);
Preferences prefs;
//* ===== LED NeoPixel =====
#define LED_PIN 48  // Pino conectado ao DIN do LED WS2812
#define NUMPIXELS 1   // Quantidade de LEDs na fita/módulo

Adafruit_NeoPixel pixels(1, 48, NEO_GRB);

//* ===== VARIÁVEIS =====
bool ledOn = true;
int red = 0, green = 0, blue = 0;

//* ===== Aplica cor =====
void applyLED() {
 if (ledOn) {
    pixels.setPixelColor(0, pixels.Color(red, green, blue));
 } else {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
 }
    pixels.show();
 
}

// ===== Salvar/Carregar estado =====
void saveState() {
  prefs.begin("ledrgb", false);
  prefs.putBool("ledOn", ledOn);
  prefs.putInt("red", red);
  prefs.putInt("green", green);
  prefs.putInt("blue", blue);
  prefs.end();
}

void loadState() {
  prefs.begin("ledrgb", true);
  ledOn = prefs.getBool("ledOn", false);
  red = prefs.getInt("red", 0);
  green = prefs.getInt("green", 0);
  blue = prefs.getInt("blue", 0);
  prefs.end();
  applyLED();
}

// ===== Publicar estado atual =====
// void publishState() {
//   JsonDocument doc;
//   doc["on"] = ledOn;
//   doc["r"] = red;
//   doc["g"] = green;
//   doc["b"] = blue;

//   String msg;
//   serializeJson(doc, msg);
//    mqtt.publish(mqtt_pub, msg.c_str());
//   Serial.println(" Estado publicado: " + msg);
//}

// ===== Callback MQTT =====
void callback(char *topic, byte *payload, unsigned int length) {
  String msg((char *)payload, length);
// Serial.printf(" Mensagem recebida (tópico: [%s]): %s\n", topic, msg.c_str());

  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, msg);

  if (erro) {
    Serial.print(" Erro JSON: ");
    Serial.println(erro.c_str());
    return;
  }

  if (String(topic) == mqtt_sub) {
    ledOn = doc["on"];
    red = doc["r"];
    green = doc["g"];
    blue = doc["b"];

    applyLED();
    saveState();
    //publishState();
  }

  // if (String(topic) == mqttTopic) {
  //   const char *tipo = doc["tipo"];
  //   const char *mensagem = doc["mensagem"];
  //   if (strcmp(tipo, "alerta") == 0) {
  //     Serial.println("ALERTA recebido!");
  //     Serial.println(mensagem);
  //   }
  // }

  //pixels.setPixelColor(0, pixels.Color(red, green, blue));
  //pixels.show();
}

// ===== Conectar ao MQTT =====
void ConectaMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (mqtt.connect(mqtt_id)) {
      Serial.println(" Conectado!");
      mqtt.subscribe(mqtt_sub);
      mqtt.subscribe(mqttTopic);
      //publishState();
    } else {
      Serial.print("Erro: ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  conectaWiFi();

  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_CERT);
  espClient.setPrivateKey(AWS_KEY);

  mqtt.setServer(AWS_BROKER, mqtt_port);
  mqtt.setCallback(callback);

  pixels.begin();
  pixels.clear();
  pixels.show();

  loadState();
}

// ===== LOOP =====
void loop() {
  checkWiFi();

  if (!mqtt.connected()) {
    ConectaMQTT();
  }
  mqtt.loop();
}