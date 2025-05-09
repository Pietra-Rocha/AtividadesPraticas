#include <Arduino.h>

#define pinLed 2
#define pinButton 5

void setup()
{
  pinMode(5, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  bool estadoAtual = digitalRead(5);
  static bool estadoAnterior = 1;
  static bool estadoLed = 0;

  static unsigned long tempoUltimaMudanca = 0;
  unsigned long tempoAtual = millis();
  static bool ultimaAcao = 1;
  const unsigned long tempoFiltro = 50;

  //! INICIA O TRATAMENTO DE RUIDO DO BOTAO

  if (estadoAtual != estadoAnterior)                         //* Detecta mudanca no estado do botao
  {
    tempoUltimaMudanca = tempoAtual;                        //*Atualiza o tempo da ultima mudanca
  }
  if ((tempoAtual - tempoUltimaMudanca) > tempoFiltro)     //*verifica se o novo estado se manteve estavel por tempo suficiente
  {
    if (estadoAtual != ultimaAcao)                        //* Verifica se ainda nao processamos essa nova acao
    {
      ultimaAcao = estadoAtual;                          //*Atualiza o estado ja processado para evitar acoes repetidas

      if (estadoAtual == 0)                             //*Se o botao foi pressionado (nivel logico LOW)
      {
        estadoLed = !estadoLed;
        digitalWrite(2, estadoLed);
      }
      else                                            //* se o botao foi solto (nivel logico HIGH), nenhuma acao por enquanto
      {
      }
    }
  }

  estadoAnterior = estadoAtual;                     //*Atualiza o estado anterior para a proxima iteracao
  //! FINAL DO TRATAMENTO DO BOTAO
}