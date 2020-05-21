//Libraries
#include <Arduino.h>
#include "config.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

//Initialize motor
Servo servo;

//Initialize display
LiquidCrystal_I2C display(0x27, 20, 4);

void setup() {

  //Set up Encoder Pins
  pinMode(ENC_DT_PIN, INPUT);
  pinMode(ENC_CLK_PIN, INPUT);
  pinMode(ENC_BUTTON_PIN, INPUT);
  pinMode(ENC_BUTTON_PIN, INPUT_PULLUP);

  //Read current state of CLK pin
  //Assign to previous state
  previousStateCLK = digitalRead(ENC_CLK_PIN);

  //Initialize servo
  servo.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX);

  //Initialize Serial monitor
  Serial.begin(9600);

  //Initialize Display
  display.init();
  display.backlight();

  //Write initial screens
  display.setCursor(0,0);
  display.print("Foresight Systems");
  display.setCursor(0,1);
  display.print("Firmware Version RC01");
  delay(3000);
  
  //clear display, and write initial splash screen
  delayMicroseconds(100);
  display.clear();
  display.setCursor(0,0);
  display.print("Automated BVM");
  display.setCursor(0,1);
  display.print("Choose angle");
  display.setCursor(0,2);
  display.print(counter);
  delayMicroseconds(100);


}
void pump(int x){
  while(1){
    servo.write(x);
    delayMicroseconds(1000);
    servo.write(0);
    delayMicroseconds(1000);
  }
}

void loop() {

  // switch(stateVariable){
  //   case 0:
  //
  //   case 1:
  //
  //   case 2:
  //
  //   case 3:



      
  switch(stateVariable){

    case 0:

    
      currentStateCLK = digitalRead(ENC_CLK_PIN);

      while(ENC_BUTTON_PIN == HIGH){

        //If previous state is different than current, than a pulse has occured
        if(currentStateCLK != previousStateCLK){

          //if InputDT state is different than InputCLK state then
          //encoder is rotating counterclockwise, which is denoted negative
          if (digitalRead(ENC_DT_PIN) != currentStateCLK){
            counter --;
            display.setCursor(0,2);
            display.print("                    ");
            delayMicroseconds(10);
            display.setCursor(0,2);
            display.print(counter);
        }
          else{
            counter + 10;
            display.setCursor(0,2);
            display.print("              ");
            display.setCursor(0,2);
            display.print(counter);
        }
      }
    }
      stateVariable = 1;

    case 1:
      display.clear();
      display.setCursor(0,0);
      display.print("Pumping at angle:" + counter);
      display.setCursor(0,1);
      display.print("Press Button to Go Back");
      while(ENC_BUTTON_PIN == HIGH){
        pump(counter);
      }
      stateVariable = 0;

  }

}
