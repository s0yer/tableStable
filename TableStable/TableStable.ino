// Bibliotecas Adicionadas
#include <Servo.h> 
#include <Wire.h>

// variável do tipo sensor.
const int MPU=0x68;  

// variáveis para o servo motor.
Servo servoAzul;
Servo servoVerm;

// variáveis utilizadas no algorítmo de controle.
int inigyx=0, inigyy=0, anguloAzul=0, anguloVermelho=0, acumulador=0;
int iniAzul=0, iniVermelho=0;

//Variaveis para armazenar valores dos sensores do MPU6050.
int AcX=0,AcY=0,AcZ=0,Tmp,GyX=0,GyY=0,GyZ=0;

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
  servoAzul.attach(9); // cabo de sinal do servoAzul conectado na porta digital PWM de saída 9
  servoVerm.attach(10); // cabo de sinal do servoVermelho conectado na porta digital PWM de saída 10

  // angulo dos servos para equilibrio do sistema
  anguloAzul=30;
  anguloVermelho=95;

  //guarda os valores dos angulos iniciais dos dois servos em variáveis.
  iniAzul=anguloAzul;
  iniVermelho=anguloVermelho;

  //escreve os valores do angulo nos servos
  servoAzul.write(anguloAzul);
  servoVerm.write(anguloVermelho);

  // seta os valores iniciais em outra de comparação para GyY e GyX
  calcGiroscopio();
  inigyx=GyX;
  inigyy=GyY;
} 
 
void loop() 
  {     
    calcGiroscopio();
    printGiroscopio();
   
  /* Alterações no eixo Y, implicam em mudanças no servo vermelho.            
   */ 
  
   if(GyY==inigyy){ // ponto de parada, caso o valor atual do giroscópio para o eixo Y esteja = ao valor da calibração do mesmo eixo.
    anguloVermelho = iniVermelho;
   } else if(GyY<inigyy){ // compara se o valor atual do giroscópio para o eixo Y é menor que o valor da calibração do mesmo eixo.
      anguloVermelho= anguloVermelho+1; 
      } else { // condição que representa o valor atual do giroscópio maior que o valor da calibração do eixo Y. (GyY>inigyy) 
      anguloVermelho= anguloVermelho-1; 
      }
      
   /* alterações no eixo X, implicam em mudanças no servo azul.
    * 
    */
    
   if(GyX==inigyx){ // ponto de parada, caso o valor atual do giroscópio para o eixo X esteja = ao valor da calibração do mesmo eixo.
    anguloAzul= iniAzul;
   }else if(GyX<inigyx){ // compara se o valor atual do giroscópio para o eixo X é menor que o valor da calibração do mesmo eixo.
     anguloAzul= anguloAzul+1;
      }else{ // condição que representa o valor atual do giroscópio maior que o valor da calibração do eixo Y. (GyX>inigyx) 
    anguloAzul= anguloAzul-1;
      }
      
  	/* Porta 9 arduino, 
     * escreve  no servo azul o novo valor de anguloAzul
  	 */
  	servoAzul.write(anguloAzul);  //Movimenta o servo azul para a posição
  	delay(50);  //Delay para movimentar o servo

  	/* Porta 10 arduino, setado novo valor para o servo vermlho
     * 
  	 */
  	servoVerm.write(anguloVermelho);  // Movimenta o servo vermelho para nova posição
  	delay(50);  //Delay para movimentar o servo

  /* Controla erro de incrementação de angulos com uma interrupção que é ativada após 20 ciclos do algorítmo.
   * Isto é, após 20 ciclos do algoritmo principal que esta dentro do loop(), a mesa é recalibrada.
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
  Serial.print("Acel. X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.print(AcZ);
  Serial.print(" | Gir. X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.print(GyZ);
  Serial.println(" | Temp = "); Serial.println(Tmp/340.00+36.53);
  
  /*Impime valores que estão sendo escritos no servo, para aferição de valores.
   */
  Serial.println(anguloVermelho); 
  Serial.println(anguloAzul);
  }

void calcGiroscopio(){
 //inicializa os componentes
 
 /* inicializa com o registrador 0x3B (ACCEL_XOUT_H), local onde se começará a fazer a leitura de endereços.
  * A leitura neste caso vai do registrador 0x3B até 0x48.
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);   
  Wire.endTransmission(false);
  
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true); 

   /* Armazena o valor dos sensores nas variaveis correspondentes,
    *os endereços de memória são acessados 8 vezes, e cada vez o acesso 
    *recebe 2bits de valores em dois endereços distintos. No final teremos
    *16bits para cada variável.
    */
    
  AcX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  }
