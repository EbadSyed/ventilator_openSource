 //Defines
#define ENC_CLK_PIN 6
#define ENC_DT_PIN 5
#define ENC_SW_PIN 7


int counter = 0;
int state;
int Laststate; 

void setup() {
  pinMode(ENC_CLK_PIN,INPUT);
  pinMode(ENC_DT_PIN,INPUT);
  pinMode(ENC_SW_PIN,INPUT);
  Serial.begin(9600);

  Laststate = digitalRead(ENC_CLK_PIN);
}

void loop() 
{
  rot_counter();     
}


void rot_counter()
{
  state = digitalRead(ENC_CLK_PIN);
  if(state != Laststate)
  {
    if(digitalRead(ENC_DT_PIN) != state)
    {
      counter++;
    }
    else
    {
      counter--;
    }
    Serial.println(counter);
  }

  if(digitalRead(ENC_SW_PIN) != 1)
  {
    counter=0;
    Serial.println(counter);
  }

  Laststate= state;

}

  
