#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "internet.h"
#include "certificados.h"

#define LED_R 48
#define BTN_BOOT 0

// Broker MQTT
const int mqtt_port = 8883;
const char *mqtt_id = "idDoEsp32/lembretes";
const char *mqtt_sub = "senai134/mesa04";
const char *mqtt_pub = "senai134/mesa04";

// Objeto LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Conexões MQTT
WiFiClientSecure espClient;
PubSubClient mqtt(espClient);

void Callback(char *topic, byte *payLoad, unsigned int Length);
void ConectaMQTT();
void controleLED();

bool novaMensagem = false;
unsigned long ultimoBlink = 0;
bool ledLigado = false;

void setup()
{
    Serial.begin(9600);
    conectaWiFi();

    pinMode(LED_R, OUTPUT);
    pinMode(BTN_BOOT, INPUT);

    lcd.init();
    lcd.backlight();

    espClient.setCACert(AWS_ROOT_CA);
    espClient.setCertificate(AWS_CERT);
    espClient.setPrivateKey(AWS_KEY);

    mqtt.setServer(AWS_BROKER, mqtt_port);
    mqtt.setCallback(Callback);
}

void loop()
{
    if (!mqtt.connected())
    {
        ConectaMQTT();
    }
    mqtt.loop();

    controleLED();
}

void Callback(char *topic, byte *payLoad, unsigned int Length)
{
    /*String mensagem((char *)payLoad, Length);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, mensagem);
    if (!error)
    {
        const char *lembrete = doc["lembrete"];
        if (lembrete)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Lembrete:");
            lcd.setCursor(0, 1);
            lcd.print(lembrete);

            novaMensagem = true;
        }
    }*/

    String msg((char *)payLoad, Length);
    Serial.print("..........................");
    Serial.printf("mensagem recebida (topico: [%s]): %s\n\r", topic, msg.c_str());
}

void controleLED()
{
    if (!novaMensagem)
        return;

    if (digitalRead(BTN_BOOT) == LOW)
    {
        novaMensagem = false;
        digitalWrite(LED_R, LOW);
        return;
    }

    if (millis() - ultimoBlink >= 500)
    {
        ultimoBlink = millis();
        ledLigado = !ledLigado;
        digitalWrite(LED_R, ledLigado);
    }
}

void ConectaMQTT()
{
    while (!mqtt.connected())
    {
        Serial.print("Conectando ao AWS IoT core ...");
        if (mqtt.connect(mqtt_id))
        {
            Serial.println("conectado.");
            mqtt.subscribe(mqtt_sub);
        }
        else
        {
            Serial.print("Falha, rc=");
            Serial.print(mqtt.state());
            delay(2000);
        }
    }
}