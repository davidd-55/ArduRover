const int motorA_PWMSpeed = 4;  // 4
const int motorA_Direction = 5;  // PWM 5
const int motorBIn3 = 8;  // 8
const int motorBIn4 = 9;  // PWM 9

void setup() {
  // put your setup code here, to run once:
  pinMode(motorA_PWMSpeed, OUTPUT);
  pinMode(motorA_Direction, OUTPUT);
  pinMode(motorBIn3, OUTPUT);
  pinMode(motorBIn4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(motorA_PWMSpeed, LOW); // HIGH PWM + LOW Direction = forward
  digitalWrite(motorA_Direction, LOW); // HIGH PWM + HIGH Direction = ???
  digitalWrite(motorBIn3, LOW); // HIGH = actively turning ON
  digitalWrite(motorBIn4, LOW); // LOW = Left; HIGH = ???
}
