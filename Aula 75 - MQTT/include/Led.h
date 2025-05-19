#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led 
{   
    private:  //atributos/caracteristicas
        int pino;
        bool estadoLed;
        bool modoPiscar = false;
        unsigned long ultimaTroca = 0;
        unsigned long intervalo = 0;

    public:   //metodos/acoes
        Led(int pin);  //metodo contrutor tem o mesmo nome da class 

        void ligar();
        void desligar();
        void inverter();

        void piscar(float frequencia);
        void update();

        int getPino();
        unsigned long getIntervalo();
};

#endif