//* ---------------------------------------------
//*     BIBLIOTECAS  E DEFINICOES 
//* ---------------------------------------------
#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>
#define boott 0

//* -------CONFIGURAÇÃO  DO WIFI-------
const char *SSID = "SALA 10";
const char *SENHA = "info@134";
const unsigned long tempoEsperaConexao = 15000;
const unsigned long tempoEsperaReconexao = 5000;

//* -------ESTANCIAS DE OBJETOS
Timezone tempo;
WiFiUDP ntpUDP;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Bounce boot = Bounce();

//* -------PROTOTIPOS DA FUNÇÕES-------
void conectaWiFi(void);
void checkWiFi(void);


//* ---------------------------------------------
//*     INICO DAS ACOES 
//* ---------------------------------------------
void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  conectaWiFi();
  tempo.setLocation("America/Sao_paulo");
  boot.attach(boott, INPUT_PULLUP);
  waitForSync();
}

void loop()
{
  boot.update();
  checkWiFi();

  bool Selecionar = 0;
  bool opcao = 1;

  //*Exemplo de como deve aparecer no lcd
  // Serial.println(tempo.dateTime());      //* Tuesday, 06-may-2025 17:02:34 <-03>


   //* ---------------------------------------------
  //*     DIA DA SEMANA 
  //* ---------------------------------------------

  switch (tempo.weekday()) //* dia da semana domingo = 1
  {
  case 1:
    lcd.setCursor(0, 0);
    lcd.print("Domingo");
    break;

  case 2:
    lcd.setCursor(0, 0);
    lcd.print("Segunda");
    break;

  case 3:
    lcd.setCursor(0, 0);
    lcd.print("Terca");
    break;

  case 4:
    lcd.setCursor(0, 0);
    lcd.print("Quarta");
    break;

  case 5:
    lcd.setCursor(0, 0);
    lcd.print("Quinta");
    break;

  case 6:
    lcd.setCursor(0, 0);
    lcd.print("Sexta");
    break;

  case 7:
    lcd.setCursor(0, 0);
    lcd.print("Sabado");
    break;
  }

  lcd.setCursor(2, 1);
  lcd.print("/");

  //* ---------------------------------------------
  //*     DIA DO MES
  //* ---------------------------------------------

  lcd.setCursor(0, 1);
  if (tempo.day() < 10) //*Dia
  {
    lcd.printf("0%d", tempo.day());
  }
  else
  {
    lcd.print(tempo.day());
  }

  //* ---------------------------------------------
  //*     MES DO ANO 
  //* ---------------------------------------------
  switch (tempo.month()) //*mes

  {
  case 1:
    lcd.setCursor(3, 1);
    lcd.print("Janeiro");
    break;

  case 2:
    lcd.setCursor(3, 1);
    lcd.print("Fevereiro");
    break;

  case 3:
    lcd.setCursor(3, 1);
    lcd.print("Marco");
    break;

  case 4:
    lcd.setCursor(3, 1);
    lcd.print("Abril");
    break;

  case 5:
    lcd.setCursor(3, 1);
    lcd.print("Maio");
    break;

  case 6:
    lcd.setCursor(3, 1);
    lcd.print("Junho");
    break;

  case 7:
    lcd.setCursor(3, 1);
    lcd.print("Julho");
    break;

  case 8:
    lcd.setCursor(3, 1);
    lcd.print("Agosto");
    break;

  case 9:
    lcd.setCursor(3, 1);
    lcd.print("Setembro");
    break;

  case 10:
    lcd.setCursor(3, 1);
    lcd.print("Outubro");
    break;

  case 11:
    lcd.setCursor(3, 1);
    lcd.print("Novembro");
    break;

  case 12:
    lcd.setCursor(3, 1);
    lcd.print("Dezembro");
    break;
  }

  lcd.setCursor(7, 1);
  lcd.print("/");

  //* ---------------------------------------------
  //*     ANO ATUAL
  //* ---------------------------------------------
  lcd.setCursor(8, 1);
  lcd.print(tempo.year()); //*ANO

  //* ---------------------------------------------
  //*     HORA DO DIA 
  //* ---------------------------------------------
  lcd.setCursor(0, 2); //*HORA(24h)
  if (boot.fell())
  {
    opcao = !opcao;
    if (opcao)
    {
      if (tempo.hour() < 10)
      {
        lcd.printf("0%d", tempo.hour());
      }
      else
      {
        lcd.print(tempo.hour());
      }
    }
  }
  else
  {
    if (tempo.hourFormat12() < 10)  //*HORA DE 12H 
    {
      lcd.printf("0%d", tempo.hour());
    }
    else
    {
      lcd.print(tempo.hourFormat12());
    }
  }


  lcd.setCursor(2, 2);
  lcd.print(":");

  //* ---------------------------------------------
  //*     MINUTO DA HORA
  //* ---------------------------------------------
  lcd.setCursor(3, 2);
  if (tempo.minute() < 10) //* MINUTO
  {
    lcd.printf("0%d", tempo.minute());
  }
  else
  {
    lcd.print(tempo.minute());
  }

  lcd.setCursor(5, 2);
  lcd.print(":");

  //* ---------------------------------------------
  //*     SEGUNDO DO MINUTO 
  //* ---------------------------------------------
  lcd.setCursor(6, 2);

  if (tempo.second() < 10) //*SEGUNDOS
  {
    lcd.printf("0%d", tempo.second());
  }
  else
  {
    lcd.print(tempo.second());
  }

  // Serial.println(tempo.hourFormat12()); //* HORA (12h)
  // Serial.println(tempo.now());          //*EPOCH TIME SEGUNDOS quantos segundos desde 01/01/1970
  // Serial.println(tempo.dayOfYear());    //* dia do ano

  // lcd.setCursor(9, 2);
  // lcd.println(tempo.isAM());         //* verdadeiro caso for antes do meio dia
  // lcd.println(tempo.isPM());         //* verdadeiro caso for depois do meio dia

  delay(10000);
}

  //* ---------------------------------------------
  //*     FUNCOES DA CONEXAO DE WIFI 
  //* ---------------------------------------------
void conectaWiFi()
{
  Serial.printf("Conectando ao WiFi: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWiFi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWiFi < tempoEsperaConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi Conectado com sucesso! ");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  }

  else
  {
    Serial.println("\nFalha ao conectar no WiFi. Verifique o nome da rede e a senha");
  }
}

void checkWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\n Conexão Perdida! Tentando reconectar...");
    conectaWiFi();
  }
}