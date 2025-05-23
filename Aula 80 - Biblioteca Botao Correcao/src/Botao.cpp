#include "Botao.h"

/**
 * @brief construtor da classe botao
 * @param p numero do pino digital ao qual o botao esta conectado
 */
botao::botao(uint8_t p) : pino(p) {}

/**
 * @brief Inicializa o botao
 * Configura o pino como entrada. O modo pull-up padra
 */
void botao::begin()
{
    pinMode(pino, INPUT_PULLUP);
    estadoAtual = digitalRead(pino);
    estadoAnterior = estadoAtual;
    estadoConfirmado = estadoAtual;
    tempoInicioEstado = millis();
}

/**
 * @brief Define o temp de filtro(debounce) para estabilizar as leituras do botao
 * @param tempo Tempo de debounce em milissegundos
 */
void botao::configurarFiltro(unsigned long tempo)
{
    tempoFiltro = tempo;
}


/**
 * @brief 
 * 
 */
void botao::update()
{
    unsigned long agora = millis();
    estadoAtual = digitalRead(pino);
    flagAlteracao = false;
    flagPressionado = false;
    flagSolto = false;

    if (estadoAtual != estadoAnterior)
    {
        tempoUltimaMudanca = agora;
    }

    if ((agora - tempoUltimaMudanca) >= tempoFiltro)
    {
        if (estadoAtual != estadoConfirmado)
        {
            estadoConfirmado = estadoAtual;
            duracaoEstado = agora - tempoInicioEstado;
            tempoInicioEstado = agora;
            flagAlteracao = true;

            if (!estadoAtual)
            {
                flagPressionado = true;
            }
            else
            {
                flagSolto = true;
            }
        }
    }
    estadoAnterior = estadoAtual;
}


/**
 * @brief 
 * 
 */
bool botao::pressionado()
{
    return flagPressionado;
}


/**
 * @brief 
 * 
 */
bool botao::solto()
{
    return flagSolto;
}

/**
 * @return true se o estado do botao mudou(pressionado ou solto) na ultima alteracao
 * 
 */
bool botao::alteracaoEstado()
{
    return flagAlteracao;
}


/**
 * @brief retorna a duracao em milissegundos do ultimo estado mantido(pressionado ou solto)
 * @return tempo em milissegundos que o botao permaneceu no ultimo estado
 */
unsigned long botao::duracaoUltimoEstado()
{
    return duracaoEstado;
}