#include "Botao.h"

Botao::Botao(int pin)
{
    pino = pin;
    pinMode(pino, INPUT_PULLUP);
    estadoAnterior = false;
    ultimaMudanca = 0;
    estadoBotao = false;
    intervalo = 50;
    Pressionado = false;
    Solto = false;
}


/**
 * @brief Tratamento de ruido
 */
void Botao::tratamento()
{
    bool leituraAtual = digitalRead(pino) == LOW; // LOW = pressionado (com pull-up)

    if (leituraAtual != estadoAnterior)
    {
        ultimaMudanca = millis(); // Início do debounce
        estadoAnterior = leituraAtual;
    }

    if ((millis() - ultimaMudanca) >= intervalo)
    {
        if (leituraAtual != estadoBotao)
        {
            estadoBotao = leituraAtual;

            if (estadoBotao)
            {
                Pressionado = true;
            }
            else
            {
                Solto = true;
            }
        }
    }
}

bool Botao::pressionado()
{
    if (Pressionado)
    {
        Pressionado = false;
        return true;
    }
    return false;
}

bool Botao::solto()
{
    if (Solto)
    {
        Solto = false;
        return true;
    }
    return false;
}

bool Botao::pressionando()
{
    return estadoBotao;
}

void Botao::setIntervalo(unsigned long intervaloNovo)
{
    intervalo = intervaloNovo;
}

unsigned long Botao::getIntervalo()
{
    return intervalo;
}