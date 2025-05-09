#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
#define Botao1 5
#define Botao2 18
#define Botao3 19
#define Botao4 23
#define pinLed 2

byte coracao[] =
    {
        B00000,
        B11011,
        B10101,
        B10001,
        B01010,
        B00100,
        B00000,
        B00000};

void setup()
{
    lcd.init();
    lcd.createChar(0, coracao);
    lcd.backlight();
    pinMode(2, OUTPUT);
    pinMode(5, INPUT_PULLUP);
    pinMode(18, INPUT_PULLUP);
    pinMode(19, INPUT_PULLUP);
    pinMode(23, INPUT_PULLUP);
}

void loop()
{
    static bool estadoAnteriorBotao1 = 1;
    static bool estadoAnteriorBotao2 = 1;
    static bool estadoAnteriorBotao3 = 1;
    static bool estadoAnteriorBotao4 = 1;
    static bool estadoBotao = 0;
    static bool estadoLed = 0;
    static bool fase = 0;

    bool estadoAtualbotao1 = digitalRead(5);
    bool estadoAtualbotao2 = digitalRead(18);
    bool estadoAtualbotao3 = digitalRead(19);
    bool estadoAtualbotao4 = digitalRead(23);

    //***** TRATAMENTO DOS BOTOES *******
    // fase 0
    if (estadoAtualbotao1 != estadoAnteriorBotao1)
    {
        lcd.setCursor(7, 0);
        lcd.print("            ");
        lcd.setCursor(5, 1);
        lcd.print("            ");
        lcd.setCursor(4, 2);
        lcd.print("            ");
        lcd.setCursor(5, 3);
        lcd.print("            ");
    }

    // fase 1
    else if (estadoAtualbotao2 != estadoAnteriorBotao2)
    {
        lcd.setCursor(7, 0);
        lcd.write(0);
        lcd.write(0);
        lcd.write(0);
        lcd.setCursor(5, 1);
        lcd.print("CAI 2025");
    }

    // fase 2 
    else if (estadoAtualbotao3 != estadoAnteriorBotao3)
    {
        lcd.setCursor(4, 2);
        lcd.print("TITI E ORDI");
    }

    // fase 3 
    else if (estadoAtualbotao4 != estadoAnteriorBotao4)
    {
        lcd.setCursor(5, 3);
        lcd.write(0);
        lcd.print("BJOOOO");
        lcd.write(0);
        if (!fase)
        {
            fase = 1;
        }
        else
        {
            fase = 0;
        }
    }
    

    estadoAnteriorBotao1 = estadoAtualbotao1;
    estadoAnteriorBotao2 = estadoAtualbotao2;
    estadoAnteriorBotao3 = estadoAtualbotao3;
    estadoAnteriorBotao4 = estadoAtualbotao4;

    if (fase == 1)
    {
        digitalWrite(pinLed, HIGH);
        delay(300);
        digitalWrite(pinLed, LOW);
        delay(300);
    }

// declaracao da variavel; condicao; incremento
    for (int i = 0; i < 10; i++)
    {
        /* code */
    }
    
}