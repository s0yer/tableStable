#include <Servo.h> 
#include <Wire.h>
#include <SFE_MMA8452Q.h>

MMA8452Q acelerometro(0x1C);

Servo servoAzul;
Servo servoVerm;

int val, aux; 
int anguloIniAzul;
int anguloIniVermelho;

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Inicializando configurações Padrões :) ");

  acelerometro.init();
  
  // Pino de dados do servo 
  servoAzul.attach(9);
  servoVerm.attach(10);
  
  
} 
  // valores iniciais para estabilidade: x=0 , y=0, z= -1
  
void loop() 
  {     
  if (acelerometro.available()){
        acelerometro.read();
  }else{
       Serial.print("\t O Acelerometro não está acessível :/ ");
  }


  anguloIniAzul= acelerometro.cx*180;

  servoAzul.write(anguloIniAzul);  //Move o servo para o angulo de posição inicial graus
  delay(15);  //Delay para o servo atingir a posiçao

  /*if(acelerometro.cx>0){
      anguloIniAzul = anguloIniAzul - 1 ;
      
   } else if(acelerometro.cx<0){
              anguloIniAzul = anguloIniAzul + 1;
            }else{
              anguloIniAzul = anguloIniAzul;
            } */
  anguloIniVermelho= acelerometro.cy*180;
  
  servoVerm.write(anguloIniVermelho);  
  delay(15);

  /* if(acelerometro.cy>0){
      anguloIniVermelho = anguloIniVermelho - 1 ;
   } else if(acelerometro.cy<0){
              anguloIniVermelho = anguloIniVermelho + 1;
            }else{
              anguloIniVermelho = anguloIniVermelho;
            }*/
            
  printValorCelulas();
   
}

void printValorCelulas(){
  Serial.print("valor inteiro x: \t");
  Serial.print(acelerometro.x, 3);
  Serial.print("\t");

  Serial.print("valor calculado x: \t");
  Serial.print(acelerometro.cx, 3);
  Serial.println("\t");
  
  Serial.print("valor inteiro y: \t");
  Serial.print(acelerometro.y, 3);
  Serial.print("\t");

  Serial.print("valor calculado y: \t");
  Serial.print(acelerometro.cy, 3);
  Serial.println("\t");
  
  Serial.print("valor inteiro z: \t");
  Serial.print(acelerometro.z, 3);
  Serial.print("\t");

  Serial.print("valor calculado z: \t");
  Serial.print(acelerometro.cz, 3);
  Serial.println("\t");
  
}
