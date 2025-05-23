#include <Arduino.h>
#include "Botao.h"

botao btn_boot(0);

void setup() 
{
  btn_boot.begin();
  Serial.begin(9600);
}

void loop() 
{
  btn_boot.update();

  if (btn_boot.pressionado())
  {
    Serial.println("Botao Pressionado");
  }
  
  if (btn_boot.solto())
  {
    Serial.printf("O botao ficou pressionado %.02f segundos\n\r", btn_boot.duracaoUltimoEstado()/1000);
  }
  
}