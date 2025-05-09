#include <Arduino.h>

//prototipo de funcoes 
void ligaLed(int pin);                                    //definimos os prototipos das funcoes para que informe ao 
void desligaLed(int pin);                                //sistema sobre as funções, permitindo que elas sejam nomeadas 
void piscaLed(int pin, unsigned long tempo);            //antes que o sistema tenha a definição completa delas. 
int somaDeDoisN(int numeroA, int numeroB);



void setup()
{
 pinMode(2, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
  piscaLed(2, 100);                        //adicionamos a funcao e informamos dentro da funcao ja criada qual o numero do led e o tempo da acao. 
  Serial.print(somaDeDoisN(4, 8));         //pedimos que mostre no serial nossa funcao e 
}                                          //informamos dentro da funcao ja criada qual os numeros a serem somados. 






void ligaLed(int pin)                   //uma funcao vazia com uma variavel inteira que retorna um pin de Led Ligado
{
digitalWrite(pin, HIGH);
}

void desligaLed(int pin)               //uma funcao vazia com uma variavel inteira que retorna um pin de Led Desligado
{
digitalWrite(pin, LOW);
}

void piscaLed(int pin, unsigned long tempo)   //uma funcao vazia com uma variavel inteira que retorna um pin de Led Piscando 
{                                             //que possui uma variavel de tempo para definir o tempo em que o led pisca
ligaLed(pin);
  delay(tempo);
  desligaLed(pin);
  delay(tempo);
}

int somaDeDoisN(int numeroA, int numeroB)       //uma funcao inteira com duas variaveis inteiras que retorna uma soma de dois numeros. 
{
int resultado = numeroA + numeroB;
return resultado;
}