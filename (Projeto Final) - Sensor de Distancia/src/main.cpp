#include <Arduino.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup() {
  Serial.begin(9600);

  Wire.begin(8,9);  //*comunicacao i2c
  
  //* endereco padrao 0x29
  if (!sensor.init(0x29)); //*caso o sensor nao inicie 
  {
    Serial.print("Falha no sensor!!");
    while(1);
  }
  
  sensor.setAddress(0x2A);
  sensor.setMeasurementTimingBudget(20000);  //* tempo maximo de leitura do sensor em microsegundos
  sensor.startContinuous(25);               //* ativa a leitura automatica do sensor a cada 25ms
  sensor.setTimeout(100);                  //* limita p tempo de leitura com erro para nao travar 

}

void loop()
{
  uint16_t distancia = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred())
  {
   Serial.println("Timeout na leitura do sensor");
  }else{
   Serial.printf("Distacia: %.2f mm", distancia);
   delay(200);
  }
}