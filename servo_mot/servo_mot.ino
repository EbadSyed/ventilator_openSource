// Speed of servo should vary based on increasing the update interval and/or the increment

#include <Servo.h>

Servo servo;

int pos = 0;                  // current servo position 
int increment = 1;            // increment to move for each interval
int  updateInterval = 5;      // interval between updates
unsigned long lastUpdate = 0; // last update of position


void setup() {
  // put your setup code here, to run once:

  myservo.write(0); // sets the initial position   
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

}

void loop() {
  // put your main code here, to run repeatedly:
  servoSweep();
}

void servoSweep()
{
  if((millis() - lastUpdate) > updateInterval)  // time to update
  {
    lastUpdate = millis();
    pos += increment;
    servo.write(pos);
    Serial.println(pos);
    if ((pos >= 180) || (pos <= 0)) // end of sweep
    {
      // reverse direction
      increment = -increment;
    }
  }
}
