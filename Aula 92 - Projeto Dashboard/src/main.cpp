#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "DHT.h"
#include "Internet.h"
#include "certificados.h"
#include <ezTime.h>   // <— biblioteca para tempo

// ------- CONFIGURAÇÃO WIFI E MQTT ---------
const int mqtt_port = 8883;
const char *mqtt_id = "esppietrasensoresmesa02";
const char *topicsub = "esp32/sensores";
const char *topicpub = "esp32/sensores";
const char *deviceId = "esp32-academia";

WiFiClientSecure espClient;
PubSubClient client(espClient);

// -------- CONFIGURAÇÃO DHT22 --------
#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------- PINOS DE BOTÕES (lâmpadas apenas) --------
#define LAMP_SALA 5
#define LAMP_RECEP 7
#define LAMP_SALAO 16
#define LAMP_BANHEIRO 13

// -------- VARIÁVEIS DE ESTADO --------
bool lamp_sala = false;
bool lamp_recepcao = false;
bool lamp_salao = false;
bool lamp_banheiro = false;

float temperatura = -1000;
float umidade = -1000;

// -------- ESTADOS ANTERIORES --------
bool last_lamp_sala = false;
bool last_lamp_recepcao = false;
bool last_lamp_salao = false;
bool last_lamp_banheiro = false;
float last_temperatura = -1000;
float last_umidade = -1000;

// -------- BOUNCE --------
Bounce btnLampSala = Bounce();
Bounce btnLampRecep = Bounce();
Bounce btnLampSalao = Bounce();
Bounce btnLampBanheiro = Bounce();

// -------- ezTime --------
Timezone Brasil;   // cria timezone

// -------- FUNÇÃO CALLBACK MQTT --------
void callback(char* topic, byte* payload, unsigned int length) {
  String msg((char*)payload, length);

  Serial.print("Mensagem recebida (");
  Serial.print(topic);
  Serial.print("): ");
  Serial.println(msg);

  // --- Motores via MQTT (sem LEDs agora) ---
  if (msg == "motor_exaustor:1") {
    Serial.println("Motor Exaustor LIGADO");
  }
  else if (msg == "motor_exaustor:0") {
    Serial.println("Motor Exaustor DESLIGADO");
  }
  else if (msg == "motor_bicicleta:1") {
    Serial.println("Motor Bicicleta LIGADO");
  }
  else if (msg == "motor_bicicleta:0") {
    Serial.println("Motor Bicicleta DESLIGADO");
  }
  else if (msg == "motor_esteira:1") {
    Serial.println("Motor Esteira LIGADO");
  }
  else if (msg == "motor_esteira:0") {
    Serial.println("Motor Esteira DESLIGADO");
  }
}

// -------- CONECTA AO MQTT --------
void conectaMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(mqtt_id)) {
      Serial.println("Conectado");
      client.subscribe(topicsub);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

// -------- LEITURA DE BOTÕES E SENSORES --------
bool leituraInputs() {
  bool houveMudanca = false;

  btnLampSala.update();
  btnLampRecep.update();
  btnLampSalao.update();
  btnLampBanheiro.update();

  // Botões de lâmpadas
  if (btnLampSala.fell()) { lamp_sala = !lamp_sala; houveMudanca = true; }
  if (btnLampRecep.fell()) { lamp_recepcao = !lamp_recepcao; houveMudanca = true; }
  if (btnLampSalao.fell()) { lamp_salao = !lamp_salao; houveMudanca = true; }
  if (btnLampBanheiro.fell()) { lamp_banheiro = !lamp_banheiro; houveMudanca = true; }

  // Sensor DHT22
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && t != last_temperatura) { temperatura = t; houveMudanca = true; }
  if (!isnan(h) && h != last_umidade) { umidade = h; houveMudanca = true; }

  return houveMudanca;
}

// -------- ENVIO PARA MQTT (somente lâmpadas e sensores) --------
void enviaMQTT() {
  JsonDocument doc;
  doc["deviceId"] = deviceId;

  // Timestamp correto (10 dígitos em segundos desde 1970)
  doc["timestamp"] = UTC.now();

  // Também pode enviar a data/hora legível se quiser
  doc["datetime"] = Brasil.dateTime("Y-m-d H:i:s");

  // Somente envia os valores que mudaram
  if (lamp_sala != last_lamp_sala) { doc["lamp_sala_danca"] = lamp_sala; last_lamp_sala = lamp_sala; }
  if (lamp_recepcao != last_lamp_recepcao) { doc["lamp_recepcao"] = lamp_recepcao; last_lamp_recepcao = lamp_recepcao; }
  if (lamp_salao != last_lamp_salao) { doc["lamp_salao_principal"] = lamp_salao; last_lamp_salao = lamp_salao; }
  if (lamp_banheiro != last_lamp_banheiro) { doc["lamp_banheiro"] = lamp_banheiro; last_lamp_banheiro = lamp_banheiro; }

  if (temperatura != last_temperatura) { doc["temperatura"] = temperatura; last_temperatura = temperatura; }
  if (umidade != last_umidade) { doc["umidade"] = umidade; last_umidade = umidade; }

  doc["wifi"] = WiFi.status() == WL_CONNECTED ? "conectado" : "desconectado";
  doc["mqtt"] = client.connected() ? "conectado" : "desconectado";

  String mensagem;
  serializeJson(doc, mensagem);

  Serial.println(mensagem);
  client.publish(topicpub, mensagem.c_str());
}

// -------- SETUP --------
void setup() {
  Serial.begin(9600);
  dht.begin();
  conectaWifi(); // sua função da pasta Internet.h

  // TLS
  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(CERTIFICATE);
  espClient.setPrivateKey(PRIVATE_KEY);

  // MQTT
  client.setServer(AWS_BROKER, mqtt_port);
  client.setCallback(callback);

  // Botões
  pinMode(LAMP_SALA, INPUT_PULLUP);
  pinMode(LAMP_RECEP, INPUT_PULLUP);
  pinMode(LAMP_SALAO, INPUT_PULLUP);
  pinMode(LAMP_BANHEIRO, INPUT_PULLUP);

  btnLampSala.attach(LAMP_SALA); btnLampSala.interval(25);
  btnLampRecep.attach(LAMP_RECEP); btnLampRecep.interval(25);
  btnLampSalao.attach(LAMP_SALAO); btnLampSalao.interval(25);
  btnLampBanheiro.attach(LAMP_BANHEIRO); btnLampBanheiro.interval(25);

  // Inicializa ezTime
  waitForSync();  // espera sincronizar NTP
  Brasil.setLocation("America/Sao_Paulo");
  Serial.println("Relógio sincronizado com NTP!");
}

// -------- LOOP --------
void loop() {
  if (!client.connected()) {
    conectaMQTT();
  }
  client.loop();

  events(); // mantém o ezTime sincronizado em background

  static unsigned long ultimaLeitura = 0;
  const unsigned long intervaloLeitura = 250; // lê/avalia a cada 250ms

  unsigned long agora = millis();
  if (agora - ultimaLeitura >= intervaloLeitura) {
    ultimaLeitura = agora;
    if (leituraInputs()) {
      enviaMQTT();
    }
  }
}
