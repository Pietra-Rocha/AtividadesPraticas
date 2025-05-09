Aqui está o código modificado para usar a biblioteca Bounce2 sem o comando debounce.interval():

#include <Bounce2.h>

// Definimos as variáveis que vamos usar no nosso programa

// ledEstado é um array que armazena o estado de cada LED (true = ligado, false = desligado)
bool ledEstado[4] = {false, false, false, false};

// intensidadeLed é um array que armazena a intensidade de cada LED (0-100)
int intensidadeLed[4] = {0, 0, 0, 0};

// posicaoSeta é uma variável que armazena a posição da seta (0-3)
int posicaoSeta = 0;

// Definimos os pinos dos LEDs
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;
const int led4 = 12;

// Definimos os pinos dos botões
const int botaoCima = 6;
const int botaoBaixo = 7;
const int botaoEnter = 8;

// Inicializamos o LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definimos os objetos Bounce para os botões
Bounce debouncerCima = Bounce();
Bounce debouncerBaixo = Bounce();
Bounce debouncerEnter = Bounce();

// Função para inicializar o LCD
void inicializarLCD() {
  lcd.begin(16, 4);
  lcd.setCursor(0, 0);
  lcd.print("LED 1 OFF");
  lcd.setCursor(0, 1);
  lcd.print("LED 2 OFF");
  lcd.setCursor(0, 2);
  lcd.print("LED 3 OFF");
  lcd.setCursor(0, 3);
  lcd.print("LED 4 OFF");
}

// Função para mover a seta para cima
void moverSetaParaCima() {
  posicaoSeta++;
  if (posicaoSeta > 3) {
    posicaoSeta = 0;
  }
  lcd.setCursor(0, posicaoSeta);
  lcd.print(">");
}

// Função para mover a seta para baixo
void moverSetaParaBaixo() {
  posicaoSeta--;
  if (posicaoSeta < 0) {
    posicaoSeta = 3;
  }
  lcd.setCursor(0, posicaoSeta);
  lcd.print(">");
}

// Função para ligar o LED com intensidade
void ligarLED() {
  if (intensidadeLed[posicaoSeta] > 50) {
    digitalWrite(led[posicaoSeta], HIGH);
  } else {
    digitalWrite(led[posicaoSeta], LOW);
  }
}

// Função para aumentar a intensidade do LED
void aumentarIntensidadeLED() {
  intensidadeLed[posicaoSeta]++;
  if (intensidadeLed[posicaoSeta] > 100) {
    intensidadeLed[posicaoSeta] = 100;
  }
  lcd.setCursor(5, posicaoSeta);
  lcd.print(intensidadeLed[posicaoSeta]);
  ligarLED();
}

// Função para diminuir a intensidade do LED
void diminuirIntensidadeLED() {
  intensidadeLed[posicaoSeta]--;
  if (intensidadeLed[posicaoSeta] < 0) {
    intensidadeLed[posicaoSeta] = 0;
  }
  lcd.setCursor(5, posicaoSeta);
  lcd.print(intensidadeLed[posicaoSeta]);
  ligarLED();
}

void setup() {
  // Inicializamos os pinos dos LEDs como saídas
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Inicializamos os pinos dos botões como entradas
  pinMode(botaoCima, INPUT);
  pinMode(botaoBaixo, INPUT);
  pinMode(botaoEnter, INPUT);

  // Inicializamos os objetos Bounce
  debouncerCima.attach(botaoCima);
  debouncerBaixo.attach(botaoBaixo);
  debouncerEnter.attach(botaoEnter);

  // Inicializamos o LCD
  inicializarLCD();
}

void loop() {
  // Atualizamos os objetos Bounce
  debouncerCima.update();
  debouncerBaixo.update();
  debouncerEnter.update();

  // Verificamos se o botão cima foi pressionado
  if (debouncerCima.fell()) {
    moverSetaParaCima();
  }

  // Verificamos se o botão baixo foi pressionado
  if (debouncerBaixo.fell()) {
    moverSetaParaBaixo();
  }

  // Verificamos se o botão enter foi pressionado
  if (debouncerEnter.fell())
}