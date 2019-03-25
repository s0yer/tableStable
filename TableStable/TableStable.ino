// Bibliotecas Adicionadas
// Libraries Added
#include <Servo.h> 
#include <Wire.h>

// variável do tipo sensor.
// Variable of sensor Type.
const int MPU=0x68;  

// variáveis para o servo motor.
// Variables for the servo motor.
Servo servoAzul;
Servo servoVerm;

// variáveis utilizadas no algorítmo de controle.
// Variables used in the control algorithm
int inigyx=0, inigyy=0, anguloAzul=0, anguloVermelho=0, acumulador=0;
int iniAzul=0, iniVermelho=0;

//Variaveis para armazenar valores dos sensores do MPU6050.
//Variables to store values of the MPU6050 sensors.
int AcX=0,AcY=0,AcZ=0,Tmp,GyX=0,GyY=0,GyZ=0;

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Inicializando configurações Padrões :) ");

  //codigo para teste giroscopio
  //Gyroscope test code
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
   
  //Inicializa o MPU-6050
  //start the MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
  
  // Pino de dados do servo
  // Servo data pin
  servoAzul.attach(9); // cabo de sinal do servoAzul conectado na porta digital PWM de saída 9
  servoVerm.attach(10); // cabo de sinal do servoVermelho conectado na porta digital PWM de saída 10

  // Angulo dos servos para equilibrio do sistema
  // Servo angle for system balance
  anguloAzul=30;
  anguloVermelho=95;

  // Guarda os valores dos angulos iniciais dos dois servos em variáveis.
  // It stores the values of the initial angles of the two servos motors in variables.
  iniAzul=anguloAzul;
  iniVermelho=anguloVermelho;

  // Escreve os valores do angulo nos servos
  // Write the angle values on the servors motors
  servoAzul.write(anguloAzul);
  servoVerm.write(anguloVermelho);

  // Seta os valores iniciais em outra de comparação para GyY e GyX
  // Arrow the initial values in a comparison of GyY and GyX
  calcGiroscopio();
  inigyx=GyX;
  inigyy=GyY;
} 
 
void loop() 
  {     
    calcGiroscopio();
    printGiroscopio();
   
  /* Alterações no eixo Y, implicam em mudanças no servo vermelho.
  Changes in the Y axis, imply changes in the red servo.
   */ 
  
   if(GyY==inigyy){ 
     // ponto de parada, caso o valor atual do giroscópio para o eixo Y esteja = ao valor da calibração do mesmo eixo.
     // point of stopping, if the current value of the gyroscope for the Y axis is = the calibration value of the same axis.
    anguloVermelho = iniVermelho;
   } else if(GyY<inigyy){ 
     // compara se o valor atual do giroscópio para o eixo Y é menor que o valor da calibração do mesmo eixo.
     // compares if the current value of the gyroscope for the Y axis is less than the calibration value of the same axis.
      anguloVermelho= anguloVermelho+1; 
      } else { 
     // condição que representa o valor atual do giroscópio maior que o valor da calibração do eixo Y. (GyY>inigyy) 
     // condition representing the current value of the gyroscope greater than the value of the Y-axis calibration.
      anguloVermelho= anguloVermelho-1; 
      }
      
   /* alterações no eixo X, implicam em mudanças no servo azul.
    * Changes in the X axis, imply changes in the blue servo.
    */
    
   if(GyX==inigyx){ 
     // ponto de parada, caso o valor atual do giroscópio para o eixo X esteja = ao valor da calibração do mesmo eixo.
     // point of stopping, if the current value of the gyroscope for the X axis is = the calibration value of the same axis.
    anguloAzul= iniAzul;
   }else if(GyX<inigyx){ 
     // compara se o valor atual do giroscópio para o eixo X é menor que o valor da calibração do mesmo eixo.
     // compares if the current value of the gyroscope for the X axis is less than the calibration value of the same axis.
     anguloAzul= anguloAzul+1;
      }else{ 
     // condição que representa o valor atual do giroscópio maior que o valor da calibração do eixo Y. (GyX>inigyx) 
     // condition that represents the current value of the gyroscope greater than the value of the Y-axis calibration. (GyX> inigyx)
    anguloAzul= anguloAzul-1;
      }
      
  	/* Porta 9 arduino, 
     * escreve  no servo azul o novo valor de anguloAzul
  	 */
    // Port 9 Arduino,
    // writes in the blue servo the new value of anguloAzul
  
    // Movimenta o servo azul para a posição
    // move the blue servo to the position
  	servoAzul.write(anguloAzul);  
  
    // Delay para movimentar o servo
    // Delay to move the servo motor
  	delay(50);
  
  	//Porta 10 arduino, setado novo valor para o servo vermlho
    //Port 10 arduino, set new value for the red servo
    
    // Movimenta o servo vermelho para nova posição
    // Move the red servo to a new position
  	servoVerm.write(anguloVermelho);  
    
    //Delay para movimentar o servo
    //Servo response delay
  	delay(50);  

  /* Controla erro de incrementação de angulos com uma interrupção que é ativada após 20 ciclos do algorítmo.
   * Isto é, após 20 ciclos do algoritmo principal que esta dentro do loop(), a mesa é recalibrada.
   */
  /* Control angles increment error with an interrupt that is activated after 20 cycles of the algorithm.
    * That is, after 20 cycles of the main algorithm that is inside the loop (), the table is recalibrated.
    */
  
    if(acumulador!=20){
    acumulador = acumulador +1;
    }else{
    acumulador=0;
    anguloAzul=iniAzul;
    anguloVermelho=iniVermelho;
    }
    
}

void printGiroscopio(){
  
  /*  Imprime valores na serial:
   *  Printa os valores dos eixos do acelerometro, giroscópio
   *  Printa os valores de temperatura
   */
  /* Print values in serial:
    * Print the values of the axes of the accelerometer, gyroscope
    * Print temperature values
    */
  
  Serial.print("Acel. X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.print(AcZ);
  Serial.print(" | Gir. X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.print(GyZ);
  Serial.println(" | Temp = "); Serial.println(Tmp/340.00+36.53);
  
  //Impime valores que estão sendo escritos no servo, para aferição de valores.
  //Print values that are being written in the servo, for value gauging.
   
  Serial.println(anguloVermelho); 
  Serial.println(anguloAzul);
  }

void calcGiroscopio(){
 // inicializa os componentes
 // initializes components
 
 /* inicializa com o registrador 0x3B (ACCEL_XOUT_H), local onde se começará a fazer a leitura de endereços.
  * A leitura neste caso vai do registrador 0x3B até 0x48.
  */
  /* initializes with register 0x3B (ACCEL_XOUT_H), where it will start to read addresses.
   * The reading in this case goes from register 0x3B to 0x48.
   */
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);   
  Wire.endTransmission(false);
  
  // Solicita os dados do sensor
  // Request sensor data
  Wire.requestFrom(MPU,14,true); 

   /* Armazena o valor dos sensores nas variaveis correspondentes,
    *os endereços de memória são acessados 8 vezes, e cada vez o acesso 
    *recebe 2bits de valores em dois endereços distintos. No final teremos
    *16bits para cada variável.
    */
    /* Stores the value of the sensors in the corresponding variables,
     * memory addresses are accessed 8 times, and each time access
     * receives 2bits of values in two different addresses. In the end we will have
     * 16 bits for each variable.
     */
  AcX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  }
