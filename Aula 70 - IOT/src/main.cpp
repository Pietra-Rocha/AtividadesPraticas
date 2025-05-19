#include <Arduino.h>
#include <WiFi.h>

// prototipar a funcao
void conexaoWiFi(void);
void reconexaoWiFi(void);

// definicao de rede
const char *SSID = "MQTT";                       // nome da rede
const char *SENHA = "info@134";                  // senha da rede
const unsigned long tempoEsperaConexao = 15000;  // tempo que ele tentando se conectar na rede
const unsigned long tempoEsperaReconexao = 5000; // tempo de reconexão



void setup()
{
  Serial.begin(9600);
  conexaoWiFi();
}

void loop()
{
  reconexaoWiFi();
}





































void conexaoWiFi()
{
  Serial.printf("conectando ao Wifi: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWiFi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWiFi < tempoEsperaConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi conectado com sucesso!");
    Serial.print("Endereco IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFalha ao conectar no WiFi. Verifique novamente");
  }
}

void reconexaoWiFi()
{
  unsigned long tempoAtual = millis();
  static unsigned long tempoDaUltimaConexao = 0;

  if (tempoAtual - tempoDaUltimaConexao > tempoEsperaReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("\nConexão perdida! Tentando reconectar...");
      conexaoWiFi();
    }
    tempoDaUltimaConexao = tempoAtual;
  }
}