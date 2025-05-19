#include "Led.h"

Led::Led(int pin)
{
    pino = pin;
    estadoLed = false;
    pinMode(pino, OUTPUT);
    digitalWrite(pino, LOW);
}

void Led::ligar()
{
    estadoLed = true;
    modoPiscar = false;
    digitalWrite(pino, HIGH);
}

void Led::desligar()
{
    estadoLed = false;
    modoPiscar = false;
    digitalWrite(pino, LOW);
}

void Led::inverter()
{
    estadoLed = !estadoLed;
    digitalWrite(pino, estadoLed);
}

void Led::piscar(float frequencia)
{
    if (frequencia == 0)
        return;
    intervalo = 1000.0 / (frequencia * 2);
    ultimaTroca = millis();
    modoPiscar = true;
}

void Led::update()
{
    if (!modoPiscar)
        return;

    unsigned long agora = millis();
    if (agora - ultimaTroca >= intervalo)
    {
        inverter();
        ultimaTroca = agora;
    }
}

int Led::getPino()
{
    return pino;
}

unsigned long Led::getIntervalo()
{
    return intervalo;
}