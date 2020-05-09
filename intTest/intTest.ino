#define ENC_DT_PIN 5  // Can we change this pin to 2,3,21,20,19,18 ?

int count = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(ENC_DT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_DT_PIN), blink, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count);
  delay(20);

}

void blink() {
  count++;
}
