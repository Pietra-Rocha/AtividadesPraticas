#include <Arduino.h>  
#include "Internet.h" 
#include <wifi.h>
#include "senhas.h"

//**configuracao do wifi */
const unsigned long tempoEsperaConexao = 15000;
const unsigned long tempoEsperaReconexao = 5000;

void conectaWifi()
{

    Serial.printf("Conectando ao Wifi: %s", SSID);
    WiFi.begin(SSID, SENHA); // Inicia a conexão com o Wi-Fi
    unsigned long tempoInicialWiFi = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWiFi < tempoEsperaConexao)
    {
        Serial.print(" . "); // Indica que está tentando conectar
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\n Wifi conectado com sucesso !");
        Serial.print("Enderco IP:");
        Serial.println(WiFi.localIP()); // Exibe o endereço IP local
    }
    else
    {
        Serial.println("\nFalta ao conectar no Wifi. Verfique o nome da rede e a senha");
    }
}

void checkWifi() // Função para verificar o status da conexão Wi-Fi

{
    unsigned long tempoAtual = millis();
    static unsigned long tempoUltimaConexao = 0;
    // Verifica se é hora de checar a conexão Wi-Fi
    if (tempoAtual - tempoUltimaConexao > tempoEsperaReconexao)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("\n Conexao perdida! tentando reconectar");
            conectaWifi(); // Tenta reconectar ao Wi-Fi
        }

        tempoUltimaConexao = tempoAtual;
    }
}