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

  // test to start the servo in the first position balanced.
  anguloIniAzul=60;
  anguloIniVermeho=60;
  servoAzul.write(anguloIniAzul);
  servoVerm.write(anguloIniVermelho);
  
} 
  // valores iniciais para estabilidade: x=0 , y=0, z= -1
  
void loop() 
  {     
  if (acelerometro.available()){
	  
        acelerometro.read();
	  
	anguloIniAzul= acelerometro.cx*180; // calc of the new positon blue
	servoAzul.write(anguloIniAzul);  //Move blue servo to new position
	delay(15);  //Delay to move servo

  
	anguloIniVermelho= acelerometro.cy*180; // calc of the new positon red
	servoVerm.write(anguloIniVermelho);  // Move red servo to new positon
	delay(15);
	  
  }else{
       Serial.print("\t O Acelerometro não está acessível :/ ");
  }
            
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
