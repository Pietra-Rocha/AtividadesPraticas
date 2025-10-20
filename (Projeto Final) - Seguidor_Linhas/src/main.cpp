#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

//---------- OBJETO ----------
Adafruit_MCP23X17 mcp;

//----------- PROTOTIPAR FUNCOES ----------------

bool leituraBit(uint8_t valor, uint8_t bit);


void setup() {
  
  Serial.begin(9600);
  mcp.begin_I2C();

  for(int p = 0; p <= 7; p++)
  mcp.pinMode(p, INPUT);

  mcp.pinMode(8, OUTPUT);
  mcp.digitalWrite(8, HIGH); //LIGAR OS LEDS INFRARED

}

void loop()
{
  uint8_t leitura = mcp.readGPIOA();

  for(int b = 0; b <= 7; b++)
  Serial.print(leituraBit(leitura, b));

  Serial.print("\t");                       //espacamento
  Serial.println(leitura, HEX);
}

bool leituraBit(uint8_t valor, uint8_t bit)
{
  return ((valor >> bit) & 0x01);
}