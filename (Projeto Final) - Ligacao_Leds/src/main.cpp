#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_ADS1X15.h>


//---------- OBJETO ----------
Adafruit_MCP23X17 mcp;
Adafruit_ADS1X15 ads;

// Controle de tempo com millis()
unsigned long previousMillis = 0;
const unsigned long interval = 500; // intervalo de piscar (ms)
bool ledState = false;
unsigned long ultimaLeitura = 0;
const unsigned long intervaloLeitura = 10000; // 1 segundo

void setup() {
  Serial.begin(9600);
  ads.begin();

  // Inicializa o MCP23X17 via I2C
  if (!mcp.begin_I2C()) {
    Serial.println("Erro ao inicializar MCP23X17!");
    while (1);
  }

  // Configura cada pino individualmente
  mcp.pinMode(10, OUTPUT);
  mcp.pinMode(11, OUTPUT);
  mcp.pinMode(12, OUTPUT);
  mcp.pinMode(13, OUTPUT);
  mcp.pinMode(14, OUTPUT);
  mcp.pinMode(15, OUTPUT);

  // Garante que todos comecem desligados
  mcp.digitalWrite(10, LOW);
  mcp.digitalWrite(11, LOW);
  mcp.digitalWrite(12, LOW);
  mcp.digitalWrite(13, LOW);
  mcp.digitalWrite(14, LOW);
  mcp.digitalWrite(15, LOW);

  Serial.println("MCP23X17 configurado — piscando pinos 10 a 15 com millis()");
}

void loop() {
  unsigned long currentMillis = millis();

  // Verifica se passou o intervalo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;

    // Atualiza todos os pinos individualmente
    mcp.digitalWrite(10, ledState ? HIGH : LOW);
    mcp.digitalWrite(11, ledState ? HIGH : LOW);
    mcp.digitalWrite(12, ledState ? HIGH : LOW);
    mcp.digitalWrite(13, ledState ? HIGH : LOW);
    mcp.digitalWrite(14, ledState ? HIGH : LOW);
    mcp.digitalWrite(15, ledState ? HIGH : LOW);

    //Serial.print("Pinos 10 a 15: ");
    //Serial.println(ledState ? "LIGADOS" : "DESLIGADOS");
  }

    // temperatura motores
    unsigned long agora = millis();

  if (agora - ultimaLeitura >= intervaloLeitura) {
    ultimaLeitura = agora;

    int leitura0 = ads.readADC_SingleEnded(0);
    float tempC0 = leitura0 * 0.01875;
    Serial.printf("Sensor 0 -> Temperatura: %.2f °C\n", tempC0);

    int leitura1 = ads.readADC_SingleEnded(1);
    float tempC1 = leitura1 * 0.01875;
    Serial.printf("Sensor 1 -> Temperatura: %.2f °C\n", tempC1);

    int leitura2 = ads.readADC_SingleEnded(2);
    float tempC2 = leitura2 * 0.01875;
    Serial.printf("Sensor 2 -> Temperatura: %.2f °C\n", tempC2);

    int leitura3 = ads.readADC_SingleEnded(3);
    float tempC3 = leitura3 * 0.01875;
    Serial.printf("Sensor 3 -> Temperatura: %.2f °C\n", tempC3);

    Serial.println("----------------------------");
  }
}
