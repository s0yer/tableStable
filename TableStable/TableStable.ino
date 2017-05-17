#include <Servo.h> 
#include <Wire.h>
#include <SFE_MMA8452Q.h>


MMA8452Q acelerometro(0x1C);

Servo servoAzul;
Servo servoVerm;

int val, aux; 
int anguloIniAzul;
int anguloIniVermelho;

const int MPU=0x68;  

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
  
  acelerometro.init();
  
  // Pino de dados do servo 
  servoAzul.attach(9);
  servoVerm.attach(10);

  // test to start the servo in the first position balanced.
  anguloIniAzul=60;
  anguloIniVermelho=60;
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

  calcGiroscopio();
  printGiroscopio();
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
