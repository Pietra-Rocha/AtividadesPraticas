#include <Arduino.h>
#define enM0A 48
#define enM0B 47

const int8_t tabelaTransicao[4][4] = {
    //         00  01  10  11     <- estado atual
    /*00*/    { 0,  1, -1,  0},
    /*01*/    { 1,  0,  0,  1},
    /*10*/    { 1,  0,  0, -1},
    /*11*/    { 0, -1,  1,  0}};

void setup() {
  Serial.begin(9600);
  pinMode(enM0A, INPUT);
  pinMode(enM0B, INPUT);
}

void loop() 
{
  bool estadoA = gpio_get_level((gpio_num_t)48);
  bool estadoB = gpio_get_level((gpio_num_t)47);

  int estadoAtualEncoder = ((estadoA << 1) | estadoB);

  static int acumuladorMovimento = 0;
  static int estadoAnteriorEncoder = 3; 

  if (estadoAtualEncoder != estadoAnteriorEncoder) 
  {
    acumuladorMovimento += tabelaTransicao[estadoAtualEncoder][estadoAnteriorEncoder];
    estadoAnteriorEncoder = estadoAtualEncoder;

    Serial.print("Contador: ");
    Serial.println(acumuladorMovimento);
  }
}
