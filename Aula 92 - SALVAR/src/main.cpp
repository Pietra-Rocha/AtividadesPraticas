#include <Arduino.h>
#include <Bounce2.h>
#include <Preferences.h>

// ---------- DEFINICOES ----------
#define pinBotao 0


// --------- DECLARACOES DE OBJETOS -----------
Bounce botao = Bounce();
Preferences prefs ;


// --------- VARIAVEIS GLOBAIS -----------
int contagem = 0;
String nomeUsuario;

void setup()
{
  Serial.begin(9600);


  Serial.println("Novo usuario S/N");
  while(!Serial.available())
  {

  }
 
  char resposta = Serial.read();
  if(resposta == 's' || resposta == 'S')
  {
    Serial.print("Digite seu nome: ");
    while (!Serial.available())
    {

    }
    nomeUsuario = Serial.readStringUntil('\n');
    nomeUsuario.trim();
    prefs.begin("workSpace", false);
    prefs.putString("usuario", nomeUsuario);
    prefs.end();
  }
  else
  {
   prefs.begin("workSpace", true);
   nomeUsuario = prefs.getString("usuario", "anonimo");
   prefs.end();
  }
  
  Serial.printf("Usuario %s conectado \n", nomeUsuario);
  
  prefs.begin("workSpace", false); // (false) -> nao é apenas leitura, posso gravar (true) apenas para leitura
 
  contagem = prefs.getInt("contagemSalva", 0); //le o valor da chave no workspace
  prefs.end();

  botao.attach(pinBotao, INPUT_PULLUP);
  Serial.printf("Valor iniciado em %d \n", contagem);
}

void loop()
{
  botao.update();
  if(botao.fell() == true)
  {
 
    prefs.begin("workSpace", false); // Abre o workspace como gravacao e leitura
    prefs.putInt("contagemSalva", ++contagem);
    prefs.end();
    Serial.printf("Foi pressionado %d vez ", contagem);
  }
 
}
