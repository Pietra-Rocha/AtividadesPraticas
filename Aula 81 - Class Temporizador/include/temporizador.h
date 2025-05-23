#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include <Arduino.h>

class temporizador
{   
    private:  //atributos/caracteristicas
        unsigned long intervalo;
        unsigned long inicio;
        bool ativo;

    public:   //metodos/acoes
        temporizador(unsigned long tempo_ms = 1000);
        void iniciar();
        void parar();
        bool estaPronto();
        void reiniciar();
        bool estaAtivo();
        void definirIntervalo(unsigned long tempo_ms);
};

#endif