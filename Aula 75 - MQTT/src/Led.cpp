#include "Led.h"

Led::Led(int pin)
{
    pino = pin;
    estadoLed = false;
    pinMode(pino, OUTPUT);
    digitalWrite(pino, LOW);
}

/**
 * @brief liga o led
 */
void Led::ligar()
{
    estadoLed = true;
    modoPiscar = false;
    digitalWrite(pino, HIGH);
}

/**
 * @brief Desliga o led
 */
void Led::desligar()
{
    estadoLed = false;
    modoPiscar = false;
    digitalWrite(pino, LOW);
}

/**
 * @brief Inverte o estado do led
 */
void Led::inverter()
{
    estadoLed = !estadoLed;
    digitalWrite(pino, estadoLed);
}

/**
 * @brief Pisca o led
 * @param frequencia frenquencia em Hz que o Led devera piscar
 */
void Led::piscar(float frequencia)
{
    if (frequencia == 0)
        return;
    intervalo = 1000.0 / (frequencia * 2);
    ultimaTroca = millis();
    modoPiscar = true;
}

/**
 * @brief Funcao que atualiza o estado do Led no modo piscar
 */
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

//atributos
int Led::getPino()
{
    return pino;
}

unsigned long Led::getIntervalo()
{
    return intervalo;
}