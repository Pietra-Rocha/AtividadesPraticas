#ifndef BOTAO_H
#define BOTAO_H

#include <Arduino.h>

class Botao {
  private:
    int pino;
    bool estadoAnterior;        // Estado anterior (para detectar mudanças)
    bool estadoBotao;            // Estado atual do botão (pressionado ou não)
    bool Pressionado;          // evento de pressionado
    bool Solto;                // evento de solto
    unsigned long ultimaMudanca; // Tempo da última variação
    unsigned long intervalo; // Intervalo de debounce em milissegundos
    

  public:
    Botao(int pino);
    void setIntervalo(unsigned long intervaloNovo);
    void tratamento();
    bool pressionado();   // Está pressionado?
    bool solto();         // Está solto?
    bool mudanca();         // Houve mudança de estado?
    bool pressionando();
    unsigned long getIntervalo();
};

#endif
