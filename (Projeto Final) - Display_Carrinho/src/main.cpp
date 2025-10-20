#include <Arduino.h>
#include <TFT_eSPI.h>
#include "imagem.h"

//----------- INICIAR OBJETOS ----------------
TFT_eSPI tft;

void setup() 
{
  tft.init();                 //* inicia o display
  tft.setRotation(1);        //* rotaciona o conteudo do display 
  /*cores padroes
  BLACK
  BLUE
  RED
  GREEN
  CYAN
  MAGENTA
  YELLOW
  WHITE 
  ORANGE
  PINK
  PURPLE 
  GREY
  */
  tft.fillScreen(TFT_PURPLE);                   //*altera cor de fundo
  tft.setTextColor(TFT_WHITE);                 //*cor de texto
  tft.setTextFont(4);                         //*seleciona a font
  tft.drawString("Ola VW!", 10, 10);         //*"texto", posicao x, posicao y

  tft.setCursor(10, 40);
  int idade = 18;
  tft.printf("idade = %d", idade);


  /*-------RETANGULO--------*/
  //tft.drawRect(10, 50, 100, 10, TFT_RED);            //*desenha um retangulo (apenas linha)
  //tft.fillRect(10, 70, 200, 10, TFT_RED);           //*desenha um retangulo preenchido
  //tft.drawRoundRect(10, 70, 200, 50, 10, TFT_RED); //*(X1,Y1, X2,Y2, RAIO, COR)
  //tft.fillRoundRect(10, 70, 200, 50, 10, TFT_RED);   //*(X1,Y1, X2,Y2, RAIO, COR)

  /*-------CIRCULOS--------*/
  //tft.drawCircle(120, 120, 35, TFT_WHITE); //X, Y, RAIO, COR
  //tft.fillCircle(120, 120, 30, TFT_GOLD); //X, Y, RAIO, COR

  /*-------TRIANGULO--------*/
  //tft.drawTriangle(10, 200, 200, 200, 100, 100, TFT_WHITE); //(x1,y1, x2,y2, x3,y3)
  //tft.fillTriangle(10, 200, 200, 200, 100, 100, TFT_PINK); //(x1,y1, x2,y2, x3,y3)

  tft.pushImage(0, 0, 240, 240, gato);
}


void loop()
{
 
}