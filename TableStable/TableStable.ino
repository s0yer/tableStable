#include <Servo.h> 
#include <Wire.h>


const int MPU=0x68;  

Servo servoAzul;
Servo servoVerm;

int val, aux; 
int anguloAzul;
int anguloVermelho;



//Variaveis para armazenar valores dos sensores
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Inicializando configurações Padrões :) ");

  //codigo para teste giroscopio
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
   
  // Pino de dados do servo 
  servoAzul.attach(9);
  servoVerm.attach(10);

  // test to start the servo in the first position balanced.
  anguloAzul=60;
  anguloVermelho=60;
  servoAzul.write(anguloAzul);
  servoVerm.write(anguloVermelho);
  
} 
  // valores iniciais para estabilidade: x=0 , y=0, z= -1
  
void loop() 
  {     
    calcGiroscopio();
    printGiroscopio();
  	  
  	anguloAzul= 60; // calc of the new positon blue
  	servoAzul.write(anguloAzul);  //Move blue servo to new position
  	delay(15);  //Delay to move servo
  
    
  	anguloVermelho= 60; // calc of the new positon red
  	servoVerm.write(anguloVermelho);  // Move red servo to new positon
  	delay(15);
}

void printGiroscopio(){
  
  //Mostra os valores na serial
  Serial.print("Acel. X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.print(AcZ);
  Serial.print(" | Gir. X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.print(GyZ);
  Serial.print(" | Temp = "); Serial.println(Tmp/340.00+36.53);
  
  }

void calcGiroscopio(){

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true); 
  
   //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  }
