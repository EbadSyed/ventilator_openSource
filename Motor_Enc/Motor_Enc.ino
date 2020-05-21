#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define ENC_CLK_PIN 6
#define ENC_DT_PIN 5

const byte ENC_SW_PIN = 2; // Needs to be on of the interrupt pin

volatile byte ventModeState = LOW;

volatile byte dispUpdate = LOW;

// Interrupt can be only on these pins 2, 3, 18, 19, 20, 21

int breathingCount = 18;

// States to for maintaing Rotatatory Encoder
int state;
int Laststate; 

int pos = 25;                  // current servo position 
int increment = 1;            // increment to move for each interval
int  updateInterval = 5;      // interval between updates

unsigned long lastUpdate = 0; // last update of position

unsigned long lastUpdateSweep = 0; //last update of sweep

unsigned long compressInterval = 1500; //Time to wait at Max Compress position

unsigned long sweepInterval = 1000; // Time between compressions/sweeps

int maxAngle = 300; // Max angle to be compressed

int volume = 200; // volume and Max angles are related

//Init Servo
Servo servo;

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {

  pinMode(ENC_CLK_PIN,INPUT);
  pinMode(ENC_DT_PIN,INPUT);
  
  (ENC_SW_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_SW_PIN), updateFlag, RISING); // Setting Button on interrupt so we don't miss

  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  lcd.print(" Ventilator System");
  updateDisplay();
  
  Serial.begin(9600);

  servo.write(0); // sets the initial position   
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo.write(25); // set servo to 0 Compression
  
  Laststate = digitalRead(ENC_CLK_PIN);
}

void loop() {
  
  state = digitalRead(ENC_CLK_PIN); // check if ROT Encoder has changed // Put this on Interrupt after Interrupt Test
  updateCounter(); // Adjust Breathing Rate or Tidal Volume

  // Update Display only if Encoder has been adjusted 
  if(dispUpdate){
    updateDisplay();
  }

  // Update Motor Position
  servoSweep();
  
  
}


void updateCounter()
{
  if(state != Laststate)
  {
    dispUpdate = HIGH;
    if(digitalRead(ENC_DT_PIN) != state)
    {
      if(ventModeState)
      {
        breathingCount++;
      }
      else
        volume++;
      
    }
    else
    {
      if(ventModeState)
      {
        breathingCount--;
      }
      else
        volume--;
    }

   // We dont want to breach the min and max values for Breating Rate and Tidal Volume
    breathingCount = constrain(breathingCount,8,18);
    sweepInterval = map(breathingCount,8,18,7500,1000);
    sweepInterval = constrain(sweepInterval,1000,7500);

    volume = constrain(volume,150,200);
    maxAngle = map(volume,150,200,100,300);
    maxAngle = constrain(maxAngle,100,300); 
    
    Serial.print(" Breathing Count ");
    Serial.println(breathingCount);

    Serial.print(" Volume ");
    Serial.println(volume);
  }  
    Laststate= state;
}

void servoSweep()
{
  if((millis() - lastUpdate) > updateInterval)  // time to update
  {
    lastUpdate = millis();
    
    pos += increment;
    pos = constrain(pos,25,maxAngle); // We dont want to go less than 25 degrees or Max angle
    
    servo.write(pos);
    
    //Serial.print("Motor Angle  ");
    //Serial.println(pos);
    
    if ((pos >= maxAngle)) // end of sweep 
    {
      // reverse direction
      increment = -increment;
      compressInterval = millis();
    }
    else if(( pos <= 25) && (millis() - lastUpdateSweep > sweepInterval)) // We want to wait for next sweep to take place
    {
      // reverse direction
      increment = -increment;
      lastUpdateSweep = millis();
    }
  }
}

void updateDisplay()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tidal Volume");
  lcd.setCursor(14,0);
  lcd.print(volume);
  lcd.setCursor(0,2);
  lcd.print("RR ");
  lcd.setCursor(14,2);
  lcd.print(breathingCount);

  dispUpdate = LOW;
}

void updateFlag()
{
  ventModeState = !ventModeState;  // Vent Mode state to select Breathing Rate or Volume
}
