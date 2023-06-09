#include <ezButton.h>
#include <Servo.h>

const float def = 600;

int led_green = 14;
int led_blue = 12;
int led_red = 27;
int toggle = 21;
int buzz = 13;
int mq = 26;
int ser = 22;
float sensorValue;

ezButton toggleSwitch(toggle);
Servo servo;

void setup() {
  Serial.begin(9600);
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
  //for(int i = 0; i < 100; i++){
     //sensorValue = sensorValue + analogRead(mq);       // read analog input pin 0
  //}
  sensorValue = analogRead(mq);
  //sensorValue = sensorValue / 100;                  // get average reading
  //sensorVolts = sensorValue/1024*5.0;   //convert to voltage
  Serial.println(sensorValue);  // prints the value read

  //Toggle servo
  toggleSwitch.loop(); // MUST call the loop() function first

  if (toggleSwitch.isPressed()) {
    Serial.println("The switch: OFF -> ON");
    servo.write(30);
    digitalWrite(led_blue, HIGH);
  }  

  if (toggleSwitch.isReleased() || sensorValue>=def ){
    Serial.println("The switch: ON -> OFF");
    servo.write(180);
    digitalWrite(led_blue, LOW);
  }

  if (sensorValue>=def){
    digitalWrite(led_red,HIGH);
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
  }
  if (sensorValue<def){
    digitalWrite(led_red,LOW);
    
  }
  delay(300);
}
