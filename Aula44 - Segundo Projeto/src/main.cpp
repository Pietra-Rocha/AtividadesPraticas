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
  static bool contagem = 0;

  static unsigned long tempoUltimaMudanca = 0;
  unsigned long tempoAtual = millis();
  static bool ultimaAcao = 1;
  const unsigned long tempoFiltro = 50;


  //! INICIA O TRATAMENTO DE RUIDO DO BOTAO 

  //* Detecta mudanca no estado do botao 
  if (estadoAtual != estadoAnterior)
  {
    tempoUltimaMudanca = tempoAtual; //*Atualiza o tempo da ultima mudanca 
  }
  //*verifica se o novo estado se manteve estavel por tempo suficiente
  if ((tempoAtual - tempoUltimaMudanca) > tempoFiltro)
  {
    //* Verifica se ainda nao processamos essa nova acao 
    if (estadoAtual != ultimaAcao)
    {
      ultimaAcao = estadoAtual; //*Atualiza o estado ja processado para evitar acoes repetidas

      //*Se o botao foi pressionado (nivel logico LOW)
      if (estadoAtual == 0)
      {
        //botao apertado
        contagem++;
        Serial.println(contagem);
      }
      else //botao solto 
      {
        //* se o botao foi solto (nivel logico HIGH), nenhuma acao por enquanto 
      }
      
      
    }
  }
  //*Atualiza o estado anterior para a proxima iteracao 
  estadoAnterior = estadoAtual;
  //!FINAL DO TRATAMENTO DO BOTAO 
}