//Defines
#define ENC_CLK_PIN 6
#define ENC_DT_PIN 5
#define ENC_BUTTON_PIN 7
#define SERVO_PIN 9

//Variables
int counter = 0;
int currentStateCLK;
int previousStateCLK;
int stateVariable = 0;

// Limits of servo.
#define SERVO_MIN 2400
#define SERVO_MAX 550
