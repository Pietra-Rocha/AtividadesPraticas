#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

//-----------------------INSTANCIAS DE OBJETOS--------------------
Adafruit_MCP23X17 mcp;

//------------------------DEFINICOES------------------------------
#define pinM0Dir 18
#define pinM0Esq 3
#define pinM1Dir 10
#define pinM1Esq 46
#define pinM2Dir 13
#define pinM2Esq 14
#define pinM3Dir 12
#define pinM3Esq 11

#define I2C_SDA 8
#define I2C_SCL 9

#define Freq_PWM 20000  //frequencia do PWM 20hz
#define Resol_PWM 8     //resolucao do PWM em 8bits, de 0 ~ 255

#define pinSeguidorHabilitado 8 //habilita os leds infrasvermelhos do sensor 
#define erroLinha INT16_MAX 

const uint8_t pinMotor[4][2] =
{
  {pinM0Esq, pinM0Dir},
  {pinM1Esq, pinM1Dir},
  {pinM2Esq, pinM2Dir},
  {pinM3Esq, pinM3Dir}
};

const uint8_t pinSeguidor[8] = {0,1,2,3,4,5,6,7};

const uint8_t chMotor[4][2] =
{
  {0,1},
  {2,3},
  {4,5},
  {6,7}
};

//-------------------------------PROTOTIPOS----------------------
void iniciarSeguidorLinha();
void confMotores();
uint8_t lerSeguidorLinha();
float calculaErroLinha(uint8_t mascara);
uint8_t porcentagemPWM(uint8_t velocidade);
void acionarMotor(int indiceMotor, int velocidadePercentual);
void acionaRodasOmini(float vx, float vy, float omega);
void pararTodosMotores();
float atualizarPID(float erro, float dt);

//-------------------------------VARIAVEIS------------------------------
float kp = 4.0f;
float ki = 0.5f;
float kd = 1.2f;
float velocidadeFrente = 20.0f;
float omegaMax = 40.0f;
float erroAnterior = 0.0f;
float integralAcumulada = 0.0f;
uint32_t instanteAnteriorMs = 0;





void setup() 
{
 Serial.begin(115200);
 iniciarSeguidorLinha();
 confMotores();
}

void loop() 
{
 uint32_t agoraMs = millis();
 float dt = (agoraMs - instanteAnteriorMs) / 1000.0f;
 instanteAnteriorMs = agoraMs;

 uint8_t mascara = lerSeguidorLinha();
 int erro = calculaErroLinha(mascara);

 if (erro == erroLinha)  //nao identificou nenhuma linha
 {
  acionaRodasOmini(0.0f, 10.0f, erroAnterior > 0 ? 12.0f : -12.0f);
  return;
 }
 
 float omega = atualizarPID((float)erro, dt);
 acionaRodasOmini(0.0f, velocidadeFrente, omega);
}






void iniciarSeguidorLinha()
{
  for (char i = 0; i <= 7; i++)
    mcp.pinMode(pinSeguidor[i], INPUT);

  mcp.pinMode(pinSeguidorHabilitado, OUTPUT);
  mcp.digitalWrite(pinSeguidorHabilitado, HIGH);
}

void confMotores()
{
  for (char i = 0; i < 4; i++)
    for (char j = 0; j < 2; j++)
    {
      pinMode(pinMotor[i][j], OUTPUT);
      ledcSetup(chMotor[i][j], Freq_PWM, Resol_PWM);
      ledcAttachPin(pinMotor[i][j], chMotor[i][j]);
      ledcWrite(chMotor[i][j], 0);
    }
}

uint8_t lerSeguidorLinha()
{
 uint8_t gpa = mcp.readGPIOA();
 return gpa;
}

float calculaErroLinha(uint8_t mascara)
{
  if (mascara == 0)
    return erroLinha;  

  const int pesos[8] = {-7, -5, -3, -1, 1, 3, 5, 7};
  int somaPesos = 0;
  int contarLinha = 0;
  
  for (char i = 0; i < 8; i++)
  {
    if (mascara & (1 << i))  //avalia bit a bit qual eh 1 
    {
      //se o bit i for 1
      somaPesos += pesos[i];
      contarLinha++;
    }
  }
  return(float)somaPesos/contarLinha;
}

uint8_t porcentagemPWM(uint8_t velocidade) //velocidade de 0% ~ 100%
{
  return velocidade != 0
  ? map(velocidade, 1, 100, 150, 255)  //se nao for zero 
  : 0;                                // se for zero
}

void acionarMotor(int indiceMotor, int velocidadePercentual)
{
  uint8_t dutyCicle = porcentagemPWM(velocidadePercentual);
  if (velocidadePercentual >= 0)
  {
    ledcWrite(chMotor[indiceMotor][0], dutyCicle);
    ledcWrite(chMotor[indiceMotor][1], 0);
  }
  else
  {
    ledcWrite(chMotor[indiceMotor][0], 0);
    ledcWrite(chMotor[indiceMotor][1], dutyCicle);
  }
}

void acionaRodasOmini(float vx, float vy, float omega)
{
  float v[4];
  v[0] = vy + vx + omega;
  v[1] = vy - vx - omega;
  v[2] = vy - vx + omega;
  v[3] = vy + vx - omega;
                                                                                          //max retorna o maior entre dois numeros
  float maiorVelocidade = max(max(fabs(v[0]), fabs(v[1])), max(fabs(v[2]), fabs(v[3]))); //fabs eh o modulo de um float -> valor negativo vira positivo 


  //verifica se a maior velocidade eh maior que 100, e ajusta todas as outras proporcionalmente 
  if(maiorVelocidade > 100.0f)
    for (int i = 0; i < 4; i++)
     v[i] = (v[i] / maiorVelocidade) * 100.0f;
  
  for (int i = 0; i < 4; i++)
    acionarMotor(i, (int)roundf(v[i]));
}


//parar todos os motores
void pararTodosMotores()
{
  for (char i = 0; i < 4; i++)
  {
    ledcWrite(chMotor[i][0], 0);
    ledcWrite(chMotor[i][1], 0);
  }
}

float atualizarPID(float erro, float dt)
{
  integralAcumulada += erro * dt;
  const float limiteIntegral = 100.0f / max(0.001f, ki);

  //* limita o valor maximo da integral entre +- 100.000
  integralAcumulada = constrain(integralAcumulada, -limiteIntegral, limiteIntegral); 

  float derivada = (erro - erroAnterior) / max(dt, 0.001f);
  erroAnterior = erro;

  float omega = kp * erro + ki * integralAcumulada + kd * derivada;
  omega = constrain (omega, -omegaMax, omegaMax);

  return omega;
}
