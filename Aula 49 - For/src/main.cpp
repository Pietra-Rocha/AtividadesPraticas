#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define Botao1 5
#define Led 2

// 0 a 4 vetores.

void setup()
{

  Serial.begin(9600);

  // declaracao da variavel; condicao; incremento
  for (int tabuada = 0; tabuada <= 10; tabuada++)
  {
    for (int i = 0; i <= 10; i++)
    {
      // nu * i = resultado
      //Serial.printf("%d x %d = %d \n", tabuada, numero, numero*tabuada);
      Serial.print(tabuada);
      Serial.print("x");
      Serial.print(i);
      Serial.print("=");
      Serial.println(tabuada * i);
    }
    Serial.println("------------------");
  }
}

void loop()
{
}