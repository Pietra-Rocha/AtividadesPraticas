#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

/* Autor: Pietra Rocha
   Data: 09/04/2025
   Descricao: Aula 53 Exercicios- Correcao */

#define botaoA = 18;
#define botaoB = 19;
#define botaoC = 23;
#define Led1 = 15;
#define Led2 = 2;
#define Led3 = 4;
#define Led4 = 5;

// ---------------------------
// OBJETOS BOUNCE PARA CADA BOTÃO
// ---------------------------
Bounce debouncerCima = Bounce();  // Criamos um objeto para o botão A
Bounce debouncerBaixo = Bounce(); // Para o botão B
Bounce debouncerEnter = Bounce(); // Para o botão C

static bool posicao = 0;
bool modoSelecao = true; // TRUE = mudar seta | FALSE = mudar intensidade
int selecionar = 0;      // Guarda qual LED está selecionado (0 a 3)

// Guardamos a intensidade de 4 LEDs em um vetor
int intensidade[4] = {0, 0, 0, 0}; // começa com todos em 0



void setup()
{
  // Inicia comunicação com o monitor serial
  Serial.begin(9600);

  // Configura os botões como entrada com pull-up e Ligar os botões à biblioteca Bounce
  debouncerCima.attach(18, INPUT_PULLUP);
  debouncerBaixo.attach(19, INPUT_PULLUP);
  debouncerEnter.attach(23, INPUT_PULLUP);

  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  

  // Mensagem inicial no monitor serial
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(">LED 1: ");
  lcd.setCursor(0, 1);
  lcd.print(" LED 2: ");
  lcd.setCursor(0, 2);
  lcd.print(" LED 3: ");
  lcd.setCursor(0, 3);
  lcd.print(" LED 4: ");
}

void loop()
{
  // Atualiza os estados dos botões com a biblioteca Bounce
  debouncerCima.update();
  debouncerBaixo.update();
  debouncerEnter.update();

  // ---------------------------
  // BOTÃO C: TROCA DE MODO
  // ---------------------------
  if (debouncerEnter.fell())
  { // fell() = botão foi pressionado
    // Troca o modo
    modoSelecao = !modoSelecao;
  }

  // ---------------------------
  // BOTÃO A
  // ---------------------------
  if (debouncerCima.fell())
  {
    if (modoSelecao)
    {
      // Estamos no modo alteracao de seta para cima
      selecionar = selecionar + 1;
      if (selecionar > 3) selecionar = 0;
    }
    else
    {
      // Estamos no modo de ajuste de intensidade
      intensidade[selecionar] += 10;
      if (intensidade[selecionar] > 100)
      {
        intensidade[selecionar] = 100;
      }
      lcd.setCursor(9, selecionar);
      lcd.print(intensidade[selecionar]);
      lcd.print(" ");
    }
  }

  // ---------------------------
  // BOTÃO B
  // ---------------------------
  if (debouncerBaixo.fell())
  {
    if (modoSelecao)
    {
      // Estamos no modo de seleção de LED
      selecionar = selecionar - 1;
      if (selecionar < 0) selecionar = 3;
    }
    else
    {
      // Estamos no modo de ajuste de intensidade
      intensidade[selecionar] -= 10;
      if (intensidade[selecionar] < 0)
      {
        intensidade[selecionar] = 0;
      }
      lcd.setCursor(9, selecionar);
      lcd.print(intensidade[selecionar]);
      lcd.print(" ");
    }
  }

  //-----------------------------
  //LCD
  //----------------------------
   
   if (selecionar == 0)
   {
    lcd.setCursor(0,0);
    lcd.print(">");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(0,3);
    lcd.print(" ");
   }
   else if (selecionar == 1)
   {
    lcd.setCursor(0,1);
    lcd.print(">");
    lcd.setCursor(0,0);
    lcd.print(" ");
    lcd.setCursor(0,2);
    lcd.print(" ");   
    }
   else if (selecionar == 2)
   {
    lcd.setCursor(0,2);
    lcd.print(">");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.setCursor(0,3);
    lcd.print(" ");   
    }
   else if (selecionar == 3)
   {
    lcd.setCursor(0,3);
    lcd.print(">");
    lcd.setCursor(0,2);
    lcd.print(" ");
    lcd.setCursor(0,0);
    lcd.print(" ");  
    }
  
  //-----------------------------------
  //LEDS
  //-----------------------------------
  analogWrite(15, intensidade[selecionar] * 2.55);
  analogWrite(2, intensidade[selecionar] * 2.55);
  analogWrite(4, intensidade[selecionar] * 2.55);
  analogWrite(5, intensidade[selecionar] * 2.55);



}