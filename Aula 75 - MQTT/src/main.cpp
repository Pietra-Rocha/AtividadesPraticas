//* ---------------------------------------------
//*     BIBLIOTECAS  E DEFINICOES
//* ---------------------------------------------
#include <Arduino.h>
#include <ezTime.h>
#include "internet.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "Led.h"
#include "Botao.h"

void callback(char *, byte *, unsigned int);
void conectmqtt(void);

//* -------ESTANCIAS DE OBJETOS
Timezone tempo;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(5, DHT22);
Botao botao(0);
JsonDocument doc;
Led ledESP(2);
Led ledAmarelo(18);
Led ledVermelho(19);
Led ledBranco(21);

//* ---------------------------------------------
//*     VARIAVEIS
//* ---------------------------------------------
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_pietra";
const char *mqtt_topic_sub = "senai134/mesa02/esppietra/subs";
const char *mqtt_topic_pub = "senai134/mesa02/esppietra/public";

int tempoPisca = 500;
bool piscaLed = false;
bool estadoLed = false;

unsigned long contador = 0; // Contador de pressões do botão

int freqEsp = 3;
int freqVermelho = 3;
int freqAmarelo = 3;
int freqBranco = 3;

//* ---------------------------------------------
//*     INICIO DAS ACOES
//* ---------------------------------------------
void setup()
{
  Serial.begin(9600);

  dht.begin();

  Serial.println(dht.readHumidity());
  Serial.println(dht.readTemperature());

  conectaWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  botao.tratamento();

  if (botao.pressionado())
  {
    contador++;
    Serial.print("Botao foi pressionado : ");
    Serial.println(contador);
  }

  if (botao.pressionando())
  {
    ledESP.ligar();
  }

  if (botao.solto())
  {
    Serial.println("Botao Solto");
    ledESP.desligar();
  }

  float temperaturaLed = dht.readTemperature();
  ledESP.update();
  ledAmarelo.update();
  ledVermelho.update();
  ledBranco.update();

  checkWiFi();

  if (!client.connected())
  {
    conectmqtt();
  }

  client.loop();

  static unsigned long tempoAnterior = 0;
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior > 2000)
  {
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    // Verificação simples de faixas válidas
    if (temperatura < 0 || temperatura > 80 || umidade < 0 || umidade > 100)
    {
      Serial.println("Leitura fora do intervalo esperado!");
      return;
    }

    // Preenche o JSON
    doc["Alerta Temperatura"] = temperaturaLed;
    doc["temperatura"] = temperatura;
    doc["umidade"] = umidade;
    doc["timestamp"] = millis();

    // Criar string para armazenar o JSON
    String mensagem;
    serializeJson(doc, mensagem); // Agora escreve direto na String

    // Publica os dados
    client.publish(mqtt_topic_pub, mensagem.c_str()); // .c_str() converte String para const char*
    // Serial.print("Publicado: ");
    // Serial.println(mensagem);

    tempoAnterior = tempoAtual;
  }

  /*if (Boot.changed())
    {
      estadoBotao = !Boot.read(); // Atualiza o estado do botão

      // Cria o objeto JSON para enviar o estado do botão
      doc["estadoBotao"] = estadoBotao;

      // Serializando o JSON para string
      String mensagemBotao;
      serializeJson(doc, mensagemBotao);

      // Publicando o JSON no tópico MQTT
      client.publish(mqtt_topic_pub, mensagemBotao.c_str());
      Serial.print("Publicado: ");
      Serial.println(mensagemBotao);
    }*/

  static unsigned long tempoAnteriorPisca = 0;

  if (tempoAtual - tempoAnteriorPisca > 500)
  {
    if (temperaturaLed < 30 && temperaturaLed > 27) // LED amarelo
    {
      piscaLed = true;
      ledAmarelo.piscar(freqAmarelo);
      ledVermelho.desligar();
      ledBranco.desligar();
    }
    else if (temperaturaLed > 30) // led vermelho
    {
      piscaLed = true;
      ledVermelho.piscar(freqVermelho);
      ledAmarelo.desligar();
    }
    else if (temperaturaLed > 0 && temperaturaLed < 27) // led branco
    {
      piscaLed = true;
      ledBranco.ligar();
      ledAmarelo.desligar();
      ledVermelho.desligar();
    }
    else
    {
      ledBranco.desligar();
      ledVermelho.desligar();
      ledAmarelo.desligar();
    }
    tempoAnteriorPisca = tempoAtual;
  }
}

//* ---------------------------------------------
//*     CALLBACK DO MQTT - MENSAGEM RECEBIDA
//* ---------------------------------------------
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensagem recebida em ");
  Serial.print(topic);
  Serial.print(": ");

  // Montar a string da mensagem recebida
  String mensagem = "";
  for (int i = 0; i < length; i++)
  {
    mensagem += (char)payload[i];
  }
  Serial.println(mensagem);

  // Parse do JSON
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    Serial.printf("Erro %s no formato json", erro.f_str());
  }
  else
  {
    if (!doc["estadoLed"].isNull())
    {
      int acaoLed = doc["estadoLed"];

      switch (acaoLed)
      {
      case 0:
        ledESP.desligar();
        ledAmarelo.desligar();
        ledVermelho.desligar();
        ledBranco.desligar();
        break;

      case 1:
        ledESP.ligar();
        ledAmarelo.ligar();
        ledVermelho.ligar();
        ledBranco.ligar();
        break;

      case 2:
        if (!doc["frequenciaLed"].isNull())
        {
          JsonObject freq = doc["frequenciaLed"];

          if (freq["esp"])
            freqEsp = freq["esp"];
          if (freq["vermelho"])
            freqVermelho = freq["vermelho"];
          if (freq["amarelo"])
            freqAmarelo = freq["amarelo"];
          if (freq["branco"])
            freqBranco = freq["branco"];
        }

        ledESP.piscar(freqEsp);
        ledVermelho.piscar(freqVermelho);
        ledAmarelo.piscar(freqAmarelo);
        ledBranco.piscar(freqBranco);
        break;

      default:
        Serial.print("opcao nao implementada");
        break;
      }
    }

    if (!doc["tempo"].isNull())
    {
      tempoPisca = doc["tempo"]; // Alterando o tempo de piscamento
      Serial.print("Novo tempo de piscamento: ");
      Serial.println(tempoPisca);
    }
  }
}

//* ---------------------------------------------
//*     CONECTAR AO MQTT
//* ---------------------------------------------
void conectmqtt()
{
  while (!client.connected())
  {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(mqtt_client_id))
    {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic_sub);
    }
    else
    {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}
