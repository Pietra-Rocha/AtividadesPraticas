//* ---------------------------------------------
//*     BIBLIOTECAS  E DEFINICOES
//* ---------------------------------------------
#include <Arduino.h>
#include <ezTime.h>            // Biblioteca para manipulação de tempo com fuso horário
#include "internet.h"          // Arquivo externo com função de conexão WiFi
#include <LiquidCrystal_I2C.h> // Biblioteca para controle do display LCD I2C
#include <Bounce2.h>           // Biblioteca para tratar o bounce dos botões
void display(void);
void atualizaTempo(void);

//* -------ESTANCIAS DE OBJETOS
Timezone tempo;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Bounce botaoBoot = Bounce();

//* ---------------------------------------------
//*     VARIAVEIS
//* ---------------------------------------------
int dia = 0, mes = 0, ano = 0;
int hora = 0;
int minutoAnterior = 0, minutoAtual = 0;
int segundoAnterior = 0, segundoAtual = 0;
int diaSemana = 0, diaAno = 0;
bool formato24h = true; // Flag que indica se o horário está em formato 24h
bool manha = false;     // Flag para AM/PM no formato 12h

const char *diaSemanaTexto[] = {
    "Domingo",
    "Segunda",
    "Terca",
    "Quarta",
    "Quinta",
    "Sexta",
    "Sabado"};
const char *mesTexto[] = {
    "Jan",
    "Fev",
    "Mar",
    "Abr",
    "Mai",
    "Jun",
    "Jul",
    "Ago",
    "Set",
    "Out",
    "Nov",
    "Dez"};

//* ---------------------------------------------
//*     INICO DAS ACOES
//* ---------------------------------------------
void setup()
{
  Serial.begin(9600);                     // Inicializa comunicação serial para debug
  botaoBoot.attach(0, INPUT_PULLUP);      // Conecta o botão ao pino 0 com resistor pull-up
  conectaWiFi();                          // Conecta à internet (função definida no arquivo "internet.h")
  tempo.setLocation("America/Sao_Paulo"); // Define o fuso horário
  waitForSync();                          // Aguarda sincronização com o servidor NTP
  atualizaTempo();                        // Coleta os dados iniciais de tempo
  lcd.init();                             // Inicializa o LCD
  lcd.backlight();                        // Liga o backlight do LCD
  display();                              // Exibe as informações iniciais no LCD
}

void loop()
{
  botaoBoot.update(); // Atualiza o estado do botão
  // Verifica se o botão foi pressionado 
  if (botaoBoot.fell())
  {
    formato24h = !formato24h; // Alterna entre formato 24h e 12h
    manha = tempo.isAM();     // Atualiza se é AM ou PM
    if (!formato24h)
    {
      // Exibe hora no formato 12h com AM/PM
      lcd.setCursor(9, 2);
      lcd.printf("%s", manha ? "AM" : "PM");
      lcd.setCursor(0, 2);
      lcd.printf("%02d", tempo.hourFormat12());
    }
    else
    {
      // Exibe hora no formato 24h
      lcd.setCursor(9, 2);
      lcd.print(" "); // Limpa o campo AM/PM
      lcd.setCursor(0, 2);
      lcd.printf("%02d", tempo.hour());
    }
  }
  checkWiFi(); // Verifica e mantém a conexão WiFi
  // Atualiza os segundos e minutos atuais
  segundoAtual = tempo.second();
  minutoAtual = tempo.minute();
  manha = tempo.isAM(); // Atualiza o estado AM/PM continuamente
  // Atualiza os segundos no display se houve mudança
  if (segundoAtual != segundoAnterior)
  {
    lcd.setCursor(6, 2);
    lcd.printf("%02d", segundoAtual);
    segundoAnterior = segundoAtual;
  }
  // Quando o minuto passa de 59 para 0 (nova hora), atualiza toda a data/hora
  if (minutoAnterior == 59 && minutoAtual == 0)
  {
    atualizaTempo(); // Atualiza data, hora e estado AM/PM
    display();       // Reexibe todas as informações no LCD
  }
  // Atualiza os minutos no display se houve mudança
  if (minutoAtual != minutoAnterior)
  {
    lcd.setCursor(3, 2);
    lcd.printf("%02d", minutoAtual);
    minutoAnterior = minutoAtual;
  }
}
// Atualiza todas as variáveis com os dados atuais de tempo
void atualizaTempo()
{
  dia = tempo.day();
  mes = tempo.month();
  ano = tempo.year();
  hora = formato24h ? tempo.hour() : tempo.hourFormat12();
  diaSemana = tempo.weekday();
  diaAno = tempo.dayOfYear();
  manha = tempo.isAM();
}
// Mostra todas as informações formatadas no LCD
void display()
{
  lcd.setCursor(0, 0);
  lcd.printf("%02d/%s/%04d", dia, mesTexto[mes - 1], ano); // Exibe data
  lcd.setCursor(16, 0);
  lcd.printf("%03d", diaAno); // Exibe o dia do ano
  lcd.setCursor(0, 1);
  lcd.print(diaSemanaTexto[diaSemana - 1]); // Exibe o dia da semana
  lcd.setCursor(0, 2);
  lcd.printf("%02d:%02d:%02d", hora, minutoAtual, segundoAtual); // Exibe hora completa
}