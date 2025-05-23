#ifndef BOTAO_H
#define BOTAO_H

#include <Arduino.h>

class botao {
  private:
    uint8_t pino;
    bool estadoAnterior = HIGH;        // Estado anterior (para detectar mudanças)
    bool estadoAtual = HIGH;            // Estado atual do botão (pressionado ou não)
    bool estadoConfirmado = HIGH;          // evento de pressionado
    bool flagAlteracao = false;
    bool flagPressionado = false;
    bool flagSolto = false;
    
    unsigned long tempoInicioEstado = 0; // Tempo da última variação
    unsigned long tempoFiltro = 50; // Intervalo de debounce em milissegundos
    unsigned long tempoUltimaMudanca = 0;
    unsigned long duracaoEstado = 0;
    

  public:
    botao(uint8_t p);
    void begin();
    void update();
    
    bool pressionado();   // Está pressionado?
    bool solto();         // Está solto?
    bool alteracaoEstado();
    unsigned long duracaoUltimoEstado();

    void configurarFiltro(unsigned long tempo);
    
};

#endif