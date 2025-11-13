#include <Arduino.h>
#include <WiFi.h>              // Conexão Wi-Fi
#include <PubSubClient.h>      // MQTT
#include <Wire.h>              // I2C
#include <Adafruit_ADS1X15.h>  // Sensor de temperatura ADS1115
#include <Adafruit_MCP23X17.h> // Expansor de pinos MCP23X17
#include "Adafruit_VL53L0X.h"  // Sensor de distância
#include "seguidorlinha.h"          // Biblioteca do carrinho
#include <senhas.h>            // SSID e senha Wi-Fi
#include <certificados.h>      // Certificados (se necessário)
#include <ArduinoJson.h>       // Para enviar telemetria JSON

// ==================================================
//                CONFIGURAÇÕES DE MQTT
// ==================================================
const int mqtt_port = 8883;
const char *mqtt_client_id = "Carrinho_Mesa01";
const char *mqtt_topic_sub = "senai134/comandos";
const char *mqtt_topic_pub = "senai134/comandos";
const char *mqttTopic = "senai134/comandos";


// ==================================================
//                DEFINIÇÕES DE PINOS
// ==================================================
#define pinM0dir 14
#define pinM0Esq 13
#define pinM1dir 10
#define pinM1Esq 46
#define pinM2dir 3
#define pinM2Esq 18
#define pinM3dir 12
#define pinM3Esq 11

#define frequenciaPWM 20000
#define resolucaoPWM 8
static int PINO_BOTAO_ENCODER = 19; // botão do encoder, ativo em LOW

// ==================================================
//                LEDS E TEMPORIZADORES
// ==================================================
const int leds[] = {11, 12, 13, 14, 15}; // Pinos dos LEDs
const int numLeds = 5;
unsigned long previousMillisLeds[numLeds] = {0}; // Controle de tempo de cada LED
bool ledState[numLeds] = {false};
unsigned long intervalLeds[numLeds] = {300, 500, 700, 1000, 1500}; // Intervalos individuais

// --- Controle de partida ---
static uint32_t ATRASO_PARTIDA_MS = 300; // atraso pós-largada
static uint32_t instanteInicioAtraso = 0;

// Flags de estado (sem enum)
static bool emCorrida = false;       // está seguindo a linha?
static bool emAtrasoPartida = false; // aguardando atraso pós-largada?

// ==================================================
//             TEMPERATURA E DISTÂNCIA
// ==================================================
unsigned long ultimaLeituraTemp = 0;
const unsigned long intervaloLeituraTemp = 10000; // 10 segundos

unsigned long ultimaLeituraDist = 0;
const unsigned long intervaloLeituraDist = 1000; // 1 segundo

// ==================================================
//                MAPEAMENTO DE MOTORES
// ==================================================
const uint8_t pinMotor[4][2] = {
    {pinM0Esq, pinM0dir},
    {pinM1Esq, pinM1dir},
    {pinM2Esq, pinM2dir},
    {pinM3Esq, pinM3dir}};

const uint8_t chMotor[4][2] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7}};

const uint8_t velocidadeMax = 200; // Velocidade máxima
const uint8_t incremento = 5;      // Passo de rampa
const int tempoRampa = 20;         // Tempo entre incrementos da rampa
const int tempoMovimento = 2000;   // Tempo de movimento em cada comando

// ==================================================
//                OBJETOS DOS COMPONENTES
// ==================================================
Adafruit_ADS1115 ads;           // Sensor de temperatura
Adafruit_MCP23X17 mcp;          // Expansor de pinos
Adafruit_VL53L0X lox;           // Sensor de distância
Carrinho carrinho(mcp);         // Objeto do carrinho
JsonDocument doc;               // Para telemetria
WiFiClient espClient;           // Cliente Wi-Fi
PubSubClient client(espClient); // Cliente MQTT

// ==================================================
//                PROTÓTIPOS DE FUNÇÕES
// ==================================================
void reconnectMQTT();
void callback(char *topic, byte *message, unsigned int length);
void publicarTelemetria(float temp, int dist);
void Leds();
void temperaturaMotores();
void Leituraproximidade();
void aplicarDirecao(int dir[4]);
void rampaAcelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4]);
void rampaDesacelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4]);
void frente();
void tras();
void direita();
void esquerda();
void girarEixo();
void parar();
void pararCarrinho();
bool botaoEncoderPressionadoBorda();
bool lerBotaoEncoderBruto();
void pararCarrinho();
void seguirLinhaComCurva90();

// ==================================================
//                  FUNCOES SETUP E LOOP
// ==================================================
void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  }

  pinMode(PINO_BOTAO_ENCODER, INPUT_PULLUP); // ativo em LOW

  Wire.begin();
  mcp.begin_I2C();
  for (uint8_t i = 0; i < 8; ++i)
    mcp.pinMode(i, INPUT);

  carrinho.begin();
  carrinho.setPID(6.0f, 0.5f, 0.5f);    // valores PID iniciais kp, ki, kd
  carrinho.setVelocidades(25.0f, 0.0f); // velocidades vy, vx

  Serial.println("=== Modo BOTAO (ENCODER) ===");
  Serial.println("Inicia: estar sobre a faixa central + pressionar o botao.");
  Serial.println("Parar: pressionar o botao novamente.");
}
#define CALIBRACAO true

void loop()
{

  if (CALIBRACAO)
  {
    carrinho.tick();
    return;
  }

  else  //modo manual 
  {
    const uint8_t mascaraSensores = carrinho.lerLinhaMascara();
    const bool faixaCentralDetectada = carrinho.detectaCentro(mascaraSensores);

    // Aguardando condição de largada
    if (!emCorrida && !emAtrasoPartida)
    {
      if (faixaCentralDetectada && botaoEncoderPressionadoBorda())
      {
        emAtrasoPartida = true;
        instanteInicioAtraso = millis();
        carrinho.controlarRodas(0.0f, 0.0f, 0.0f);
        Serial.println(">> LARGADA acionada, aguardando atraso...");
      }
      else
      {
        carrinho.controlarRodas(0.0f, 0.0f, 0.0f);
      }
    }

    // Atraso pós-largada (para estabilizar)
    if (emAtrasoPartida)
    {
      if (botaoEncoderPressionadoBorda())
      {
        pararCarrinho();
      }
      else if (millis() - instanteInicioAtraso >= ATRASO_PARTIDA_MS)
      { 
        emAtrasoPartida = false;
        emCorrida = true;
        Serial.println(">> INICIOU corrida");
      }
      else
      {
        carrinho.controlarRodas(0.0f, 0.0f, 0.0f);
      }
    }

    // Seguimento ativo
    if (emCorrida)
    {
      if (botaoEncoderPressionadoBorda())
      {
        pararCarrinho();
      }
      else 
      {
        carrinho.seguirLinhaStep(); // PID+telemetria
      }
    }
  }
}


// ==================================================
//                 FUNÇÕES MQTT
// ==================================================
void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect(mqtt_client_id))
    {
      Serial.println("Conectado!");
      client.subscribe(mqtt_topic_sub);
    }
    else
    {
      Serial.printf("Falhou, rc=%d. Tentando novamente em 5s...\n", client.state());
      delay(5000);
    }
  }
}

void callback(char *topic, byte *message, unsigned int length)
{
  String comando;
  for (int i = 0; i < length; i++)
    comando += (char)message[i];
  comando.trim();
  Serial.printf("Comando recebido: %s\n", comando.c_str());

  // Executa comando recebido via MQTT
  if (comando == "frente")
    frente();
  else if (comando == "tras")
    tras();
  else if (comando == "direita")
    direita();
  else if (comando == "esquerda")
    esquerda();
  else if (comando == "parar")
    parar();
  else if (comando == "girar")
    girarEixo();
}

void publicarTelemetria(float temp, int dist)
{
  JsonDocument doc;
  doc["temperatura"] = temp;
  doc["distancia"] = dist;
  char buffer[128];
  serializeJson(doc, buffer);
  client.publish(mqtt_topic_pub, buffer);
}

// ==================================================
//                 FUNÇÕES DE LEDS
// ==================================================
void Leds()
{
  unsigned long currentMillis = millis();

  // Atualiza LEDs individualmente conforme intervalo definido
  for (int i = 0; i < numLeds; i++)
  {
    if (currentMillis - previousMillisLeds[i] >= intervalLeds[i])
    {
      previousMillisLeds[i] = currentMillis;
      ledState[i] = !ledState[i];
      mcp.digitalWrite(leds[i], ledState[i] ? HIGH : LOW);
    }
  }
}

// ==================================================
//             SENSOR DE TEMPERATURA
// ==================================================
void temperaturaMotores()
{
  if (millis() - ultimaLeituraTemp >= intervaloLeituraTemp)
  {
    ultimaLeituraTemp = millis();
    for (int ch = 0; ch < 4; ch++)
    {
      int leitura = ads.readADC_SingleEnded(ch);
      float tempC = leitura * 0.01875;
      Serial.printf("Sensor %d -> Temperatura: %.2f °C\n", ch, tempC);
      publicarTelemetria(tempC, -1);
    }
    Serial.println("----------------------------");
  }
}

// ==================================================
//             SENSOR DE DISTÂNCIA
// ==================================================
void Leituraproximidade()
{
  if (millis() - ultimaLeituraDist >= intervaloLeituraDist)
  {
    ultimaLeituraDist = millis();
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4)
    {
      int dist = measure.RangeMilliMeter;
      Serial.printf("Distância: %d mm\n", dist);
      publicarTelemetria(-1, dist);
    }
    else
      Serial.println("Distância fora de alcance");
  }
}

// ==================================================
//                 FUNÇÕES DE MOTORES
// ==================================================
void aplicarDirecao(int dir[4])
{
  for (int i = 0; i < 4; i++)
  {
    if (dir[i] == 1)
      ledcWrite(chMotor[i][0], 0), ledcWrite(chMotor[i][1], velocidadeMax);
    else if (dir[i] == -1)
      ledcWrite(chMotor[i][0], velocidadeMax), ledcWrite(chMotor[i][1], 0);
    else
      ledcWrite(chMotor[i][0], 0), ledcWrite(chMotor[i][1], 0);
  }
}

void rampaAcelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4])
{
  for (int v = 0; v <= velocidadeMax; v += incremento)
  {
    for (int i = 0; i < 4; i++)
    {
      if (dir[i] == 1)
        ledcWrite(canal[i][0], 0), ledcWrite(canal[i][1], v);
      else if (dir[i] == -1)
        ledcWrite(canal[i][0], v), ledcWrite(canal[i][1], 0);
      else
        ledcWrite(canal[i][0], 0), ledcWrite(canal[i][1], 0);
    }
    delay(tempoRampa);
  }
}

void rampaDesacelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4])
{
  for (int v = velocidadeMax; v >= 0; v -= incremento)
  {
    for (int i = 0; i < 4; i++)
    {
      if (dir[i] == 1)
        ledcWrite(canal[i][0], 0), ledcWrite(canal[i][1], v);
      else if (dir[i] == -1)
        ledcWrite(canal[i][0], v), ledcWrite(canal[i][1], 0);
      else
        ledcWrite(canal[i][0], 0), ledcWrite(canal[i][1], 0);
    }
    delay(tempoRampa);
  }
}

// ==================================================
//             MOVIMENTOS DO CARRINHO
// ==================================================
void frente()
{
  Serial.println("Frente");
  int dir[4] = {1, 1, 1, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}
void tras()
{
  Serial.println("Trás");
  int dir[4] = {-1, -1, -1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}
void direita()
{
  Serial.println("Direita");
  int dir[4] = {1, -1, -1, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}
void esquerda()
{
  Serial.println("Esquerda");
  int dir[4] = {-1, 1, 1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}
void girarEixo()
{
  Serial.println("Girar no próprio eixo");
  int dir[4] = {1, -1, 1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}
void parar()
{
  Serial.println("Parar");
  int dir[4] = {0, 0, 0, 0};
  aplicarDirecao(dir);
}


// --- Botão: debounce e detecção de borda ---
bool lerBotaoEncoderBruto()
{
  return digitalRead(PINO_BOTAO_ENCODER) == LOW;
}

bool botaoEncoderPressionadoBorda()
{
  static bool estadoEstavel = false; // false = solto, true = pressionado
  static bool ultimoEstavel = false;
  static uint32_t instanteMudanca = 0;

  bool agora = lerBotaoEncoderBruto();
  if (agora != estadoEstavel)
  {
    if (millis() - instanteMudanca >= 30)
    { // debounce 30 ms
      ultimoEstavel = estadoEstavel;
      estadoEstavel = agora;
      instanteMudanca = millis();
      return (estadoEstavel == true && ultimoEstavel == false); // borda de pressão
    }
  }
  else
  {
    instanteMudanca = millis();
  }
  return false;
}

void pararCarrinho()
{
  emCorrida = false;
  emAtrasoPartida = false;
  carrinho.controlarRodas(0.0f, 0.0f, 0.0f);
  Serial.println(">> PAROU (botao)");
}

// ==================================================
//    Função especial para seguir caminho com curvas de 90°
// ==================================================
// ==================================================
//    Função que segue linha normalmente e trata curvas de 90°
// ==================================================
void seguirLinhaComCurva90()
{
  static bool modoCurva = false;  // flag para indicar que está atravessando a curva lateralmente

  uint8_t mascara = carrinho.lerLinhaMascara();

  // =====================
  // 1️⃣ Caso NORMAL — segue linha com PID
  // =====================
  if (!modoCurva)
  {
    // Verifica se encontrou início da curva
    if (mascara == B00011111)
    {
      Serial.println("Curva 90° detectada → iniciando movimento lateral para direita");
      modoCurva = true;
      carrinho.controlarRodas(0.0f, 20.0f, 0.0f); // movimento lateral direita
      delay(100);
      return;
    }

    // Caso contrário, segue linha normalmente
    carrinho.seguirLinhaStep();
  }

  // =====================
  // 2️⃣ MODO CURVA — movimento lateral até reencontrar a linha
  // =====================
  else
  {
    if (mascara == B11111111)
    {
      // Continua lateral
      carrinho.controlarRodas(0.0f, 20.0f, 0.0f);
    }
    else if (mascara == B11111000)
    {
      // Encontrou fim da curva
      Serial.println("Fim da curva 90° → retomando seguimento normal");
      modoCurva = false;
      carrinho.controlarRodas(0.0f, 0.0f, 0.0f);
      delay(150);
    }
    else
    {
      // Caso intermediário, mantém movimento lateral suave
      carrinho.controlarRodas(0.0f, 15.0f, 0.0f);
    }
  }
}

