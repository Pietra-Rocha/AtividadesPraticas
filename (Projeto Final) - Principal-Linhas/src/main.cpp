// ==================================================
//                 BIBLIOTECAS E PASTAS
// ==================================================
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

// ==================================================
//    DEFINICOES DE PINOS E VARIAVEIS GLOBAIS
// ==================================================
#define pinM0dir 14
#define pinM0Esq 13
#define pinM1dir 10
#define pinM1Esq 46
#define pinM2dir 3
#define pinM2Esq 18
#define pinM3dir 12
#define pinM3Esq 11
#define frequenciaPWM 20000
#define resolucaoPWM 8

const uint8_t pinMotor[4][2] = { // 0 = esquerda, 1 = direita
    {pinM0Esq, pinM0dir},
    {pinM1Esq, pinM1dir},
    {pinM2Esq, pinM2dir},
    {pinM3Esq, pinM3dir}};

const uint8_t chMotor[4][2] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7}};


const uint8_t velocidadeMax = 200; // velocidade fixa para todos os movimentos
const uint8_t incremento = 5;      // incremento da rampa
const int tempoRampa = 20;         // tempo entre passos da rampa (ms)
const int tempoMovimento = 10000;  // 10 segundos

// ==================================================
//                  PROTOTIPO DE FUNCOES
// ==================================================
Adafruit_ADS1115 ads;
void temperaturaMotores();
void parar();
void rampaAcelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4]);
void rampaDesacelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4]);
void frente();
void tras();
void direita();
void esquerda();
void girarEixo();
void diagonalFrenteDireita();
void diagonalFrenteEsquerda();
void diagonalTrasDireita();
void diagonalTrasEsquerda();

//-----------------FUNCAO QUE RETORNA VALOR MAPEADO DA VELOCIDADE---------------------------
uint8_t velocidade(uint8_t valor)
{
  return valor != 0 ? map(valor, 0, 255, 150, 255) : 0;
}


// ==================================================
//                  INSTANCIAS DE OBJETOS
// ==================================================
Adafruit_ADS1115 ads;



// ==================================================
//                  FUNCOES SETUP E LOOP 
// ==================================================
void setup()
{
  Serial.begin(9600);
  ads.begin();

  for (char i = 0; i < 4; i++)
  {
    for (char j = 0; j < 2; j++)
    {
      pinMode(pinMotor[i][j], OUTPUT);
      ledcSetup(chMotor[i][j], frequenciaPWM, resolucaoPWM);
      ledcAttachPin(pinMotor[i][j], chMotor[i][j]);
    }
  }

  Serial.println("Iniciando sequência de movimentos...");

  frente();
  tras();
  direita();
  esquerda();
  girarEixo();
  diagonalFrenteDireita();
  diagonalFrenteEsquerda();
  diagonalTrasDireita();
  diagonalTrasEsquerda();

  parar();
  Serial.println("Sequência finalizada!");
}

void loop()
{
 temperaturaMotores();
}




// ==================================================
//                  FUNCOES EXTRAS 
// ==================================================
void temperaturaMotores()
{
  int leitura = ads.readADC_SingleEnded(0); // de 0 ate 3
  float tempC = leitura * 0.01875;          // converter em temperatura de LM35

  Serial.printf("Temperatura: %.2f °C \n, tempC"); // %.2f 4 um float com 2 casa decimais
  delay(1000);
}

void aplicarDirecao(int dir[4])
{
  for (int i = 0; i < 4; i++)
  {
    if (dir[i] == 1)
    {
      ledcWrite(chMotor[i][0], 0);
      ledcWrite(chMotor[i][1], velocidadeMax);
    }
    else if (dir[i] == -1)
    {
      ledcWrite(chMotor[i][0], velocidadeMax);
      ledcWrite(chMotor[i][1], 0);
    }
    else
    {
      ledcWrite(chMotor[i][0], 0);
      ledcWrite(chMotor[i][1], 0);
    }
  }
}

void rampaAcelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4])
{
  for (int v = 0; v <= velocidadeMax; v += incremento)
  {
    for (int i = 0; i < 4; i++)
    {
      if (dir[i] == 1)
      {
        ledcWrite(canal[i][0], 0);
        ledcWrite(canal[i][1], v);
      }
      else if (dir[i] == -1)
      {
        ledcWrite(canal[i][0], v);
        ledcWrite(canal[i][1], 0);
      }
      else
      {
        ledcWrite(canal[i][0], 0);
        ledcWrite(canal[i][1], 0);
      }
    }
    delay(tempoRampa);
  }
}

void rampaDesacelera(const uint8_t motor[][2], const uint8_t canal[][2], int dir[4])
{
  for (int v = velocidadeMax; v >= 0; v -= incremento)
  {
    for (int i = 0; i < 4; i++)
    {
      if (dir[i] == 1)
      {
        ledcWrite(canal[i][0], 0);
        ledcWrite(canal[i][1], v);
      }
      else if (dir[i] == -1)
      {
        ledcWrite(canal[i][0], v);
        ledcWrite(canal[i][1], 0);
      }
      else
      {
        ledcWrite(canal[i][0], 0);
        ledcWrite(canal[i][1], 0);
      }
    }
    delay(tempoRampa);
  }
}

// ==================================================
//            FUNCOES MOVIMENTOS MOTORES
// ==================================================

void frente()
{
  Serial.println("Frente");
  int dir[4] = {1, 1, 1, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void tras()
{
  Serial.println("Trás");
  int dir[4] = {-1, -1, -1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void direita()
{
  Serial.println("Direita");
  int dir[4] = {1, -1, -1, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void esquerda()
{
  Serial.println("Esquerda");
  int dir[4] = {-1, 1, 1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void girarEixo()
{
  Serial.println("Girar no próprio eixo");
  int dir[4] = {1, -1, 1, -1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void diagonalFrenteDireita()
{
  Serial.println("Diagonal Frente Direita");
  int dir[4] = {0, 1, 1, 0};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void diagonalFrenteEsquerda()
{
  Serial.println("Diagonal Frente Esquerda");
  int dir[4] = {1, 0, 0, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void diagonalTrasDireita()
{
  Serial.println("Diagonal Trás Direita");
  int dir[4] = {1, 0, 0, 1};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void diagonalTrasEsquerda()
{
  Serial.println("Diagonal Trás Esquerda");
  int dir[4] = {0, -1, -1, 0};
  rampaAcelera(pinMotor, chMotor, dir);
  delay(tempoMovimento);
  rampaDesacelera(pinMotor, chMotor, dir);
}

void parar()
{
  Serial.println("Parar");
  int dir[4] = {0, 0, 0, 0};
  aplicarDirecao(dir);
}