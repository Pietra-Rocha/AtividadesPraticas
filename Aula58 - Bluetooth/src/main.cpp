#include <Arduino.h>
#include <BluetoothSerial.h>

#define pinLed 2

BluetoothSerial BT;

void setup()
{
  pinMode(2, OUTPUT);

  BT.begin("ESPpietra");
}

void loop()
{
  static bool estadoLed = false;

  if (BT.available())
  {
    String texto = BT.readStringUntil('\n'); // vou ler o serial ate ter um enter
    Serial.printf("Voce digitou: %s\n\r", texto);
    texto.trim();

    if (texto.equals("liga"))
    {
      estadoLed = true;
      BT.println("Led ligado");
    }
    else if (texto.equals("desliga"))
    {
      estadoLed = false;
      BT.println("Led desligado");
    }
  }

  digitalWrite(2, estadoLed);
}
