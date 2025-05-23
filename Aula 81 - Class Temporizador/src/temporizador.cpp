#include "temporizador.h"

temporizador::temporizador(unsigned long tempo_ms) {
  intervalo = tempo_ms;
  inicio = 0;
  ativo = false;
}

void temporizador::iniciar() {
  inicio = millis();
  Serial.println("Temporizador iniciado com sucesso");
  ativo = true;
}

void temporizador::parar() {
  ativo = false;
}

bool temporizador::estaPronto() {
  if (!ativo) 
  {
    return false;
  }
  return (millis() - inicio >= intervalo);
}

void temporizador::reiniciar() {
  inicio = millis();
}

bool temporizador::estaAtivo() {
  return ativo;
}

void temporizador::definirIntervalo(unsigned long tempo_ms) {
  intervalo = tempo_ms;
}