#include <ezButton.h>
#include <Servo.h>
#include <MQUnifiedsensor.h>

#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define mq A0 //Analog input 0 of your arduino
#define type "MQ-6" //MQ6
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ6CleanAir 10   //RS / R0 = 10 ppm 

const float def = 400.5;

int led_green = 11;
int led_blue = 13;
int led_red = 12;
int toggle = 10;
int buzz = 8;
int ser = 9;
int count = 0;

ezButton toggleSwitch(toggle);
Servo servo;
MQUnifiedsensor MQ6(placa, Voltage_Resolution, ADC_Bit_Resolution, mq, type);

void setup() {
  Serial.begin(9600);

  MQ6.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ6.setA(2127.2); MQ6.setB(-2.526);
  MQ6.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ6.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
    Serial.print(".");
  }
  MQ6.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  //.serialDebug(true);
  Serial.println("*************** Values from MQ-6 **********************");
  Serial.println("|      LPG     |"); 
  
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buzz, OUTPUT);
  digitalWrite(led_green, HIGH);
  servo.attach(ser);
  servo.write(180);
}

void loop() {
  //MQ sensor reading
  MQ6.update();

  MQ6.setA(1009.2); MQ6.setB(-2.35);
  float LPG = MQ6.readSensor();

  Serial.print("    |    "); Serial.print(LPG);
  Serial.println("    |");
  
  //Toggle servo
  toggleSwitch.loop(); // MUST call the loop() function first

  if (toggleSwitch.isPressed()) {
    Serial.println("The switch: OFF -> ON");
    servo.write(30);
    digitalWrite(led_blue, HIGH);
  }  

  if (toggleSwitch.isReleased() || LPG>=def ){
    Serial.println("The switch: ON -> OFF");
    servo.write(180);
    digitalWrite(led_blue, LOW);
  }

  if (LPG>=def){
    digitalWrite(led_red,HIGH);
    //digitalWrite(buzz, HIGH);
    //delay(3000);
    //digitalWrite(buzz, LOW);
  }
  if (LPG<def){
    digitalWrite(led_red,LOW);
    
  }
  delay(300);
}
