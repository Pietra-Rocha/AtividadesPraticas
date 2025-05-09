#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

/* Autor: Pietra Rocha
   Data: 10/04/2025
   Descricao: Aula 56 */

/* ---------------------------------------------------------------------------------------------------- */
/*                                       AULA SOBRE STRINGS                                             */
/* -----------------------------------------------------------------------------------------------------*/

//* FORMAS DE ESCREVER PALAVRAS EM C

char palavra[10] = "OLA MUNDO";
char palavra2[10] = {'O', 'L', 'A', ' ', 'M', 'U', 'N', 'D', 'O', '\0'};

String palavra3 = "Hoje eh Quinta";

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println(palavra);
  Serial.println(palavra2);

  strcpy(palavra, "SENAI"); // substituicao de palavra

  if (strcmp(palavra, "SENAI") == 0) // se forem palavras iguais retorna 0 e se forem palavras diferentes retorna 1
  {
    Serial.println("SAO IGUAIS");
  }

  if (strncmp(palavra2, "OLA", 3) == 0) // compara c uma quantidade de caracter
  {
    Serial.println("As primeiras letras da palavra eh OLA");
  }

  if (strstr(palavra2, "MUNDO"))
  {
    Serial.println("palavra MUNDO detectada");
  }

  //-----------------------------------------

  Serial.println(palavra3.length());         // retorna o numero de letras da variavel da classe String
  Serial.println(palavra3.charAt(8));        // retorna a oitava letra da palavra (apenas uma posicao por vez)
  Serial.println(palavra3.substring(8, 14)); // retorna a oitava posicao ate a decimaquarta (retorna um intervalo de posicoes)
  Serial.println(palavra3.indexOf('u'));     // retorna a posicao da primeira ocorrencia da letra procurada.
  Serial.println(palavra3.lastIndexOf('e')); // retorna a posicao da ultima ocorrencia da letra procurada.

  if (palavra3.equals("Hoje eh Quinta") == 1) // retorna verdadeiro se forem iguais
  { Serial.println("Sao Iguais"); }

  if (palavra3.equalsIgnoreCase("hoje eh quinta") == 1) // retorna verdadeiro se forem iguais ignorando letras maiusculas ou minusculas.
  { Serial.println("Sao iguais"); }

  if (palavra3.startsWith("Hoje") == 1) //retorna verdadeiro se a palavra comecar c determinado texto
  { Serial.println("Comeca c Hoje"); }
  
  if (palavra3.endsWith("inta") == 1) //retorna verdadeiro se a palavra terminar c determinado texto
  { Serial.println("Termina c inta"); }

  String valor = "123";
  int valorNumerico = valor.toInt();  //tenta converter um texto em numero. (retorna 123,00)
  
  String valor2 = "10.5";
  float valorNumerico2 = valor2.toFloat(); //tenta converter um texto em numero decimais. (retorna 10,5)

  String mensagem = "Valor = 50";
  int tamanho =  mensagem.length(); //medir o tamanho do texto 
  String extracao = mensagem.substring(8, tamanho); //extrair o numero iniciando em uma posicao conhecida ate o fim do texto 

  Serial.println(tamanho);
  Serial.println(extracao);
  
  int numero = extracao.toInt();  //transforme o conteudo extraido em um numero inteiro
  Serial.println(numero * 2);  //utilize o numero sem moderacao... hahha 


  palavra3.toLowerCase(); //alterar todas as maiusculas para minusculas 
  Serial.println(palavra3);
  palavra3.toUpperCase(); //alterar todas as minusculas para maiusculas 
  Serial.println(palavra3);


  String textoLed = "Led = ON";
  Serial.println(textoLed);
  textoLed.replace("ON" , "OFF"); //substitui uma palavra por outra 
  Serial.println(textoLed);

  String novaFrase = "Texto     ";
  novaFrase.trim();  //remove os espacos do fim do texto

  novaFrase.concat(" adicionado"); //adiciona um texto a string 
  Serial.println(novaFrase);

  //TODO trim(), concat()

  
}

void loop()
{
}
