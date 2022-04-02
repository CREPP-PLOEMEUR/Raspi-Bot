int E1=5;
int M1=4;
int E2=6;
int M2=7;

void setup() { // put your setup code here, to run once:
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

}

void loop() { // put your main code here, to run repeatedly:
  int value;
  for(value=0; value<= 255; value+=5) {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, value);  // PWM  Speed Control
    analogWrite(E2, value);  // PWM  Speed Control
    delay(300);
  }

}
