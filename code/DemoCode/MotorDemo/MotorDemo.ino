const int driveMotorA_Direction_In1 = 4;  // 4
const int driveMotorA_PWMSpeed_In2 = 5;  // PWM 5
const int steerMotorB_In3 = 8;  // 8
const int steerMotorB_In4 = 9;  // PWM 9

const int low = 255;
const int mid = 200;
const int high = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(driveMotorA_Direction_In1, OUTPUT);
  pinMode(driveMotorA_PWMSpeed_In2, OUTPUT);
  pinMode(steerMotorB_In3, OUTPUT);
  pinMode(steerMotorB_In4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // HIGH Direction + 255 PWM Speed = forward 0 speed
  // HIGH Direction + 150 PWM Speed = forward mid speed
  // HIGH Direction + 0 PWM Speed = forward max speed
  digitalWrite(driveMotorA_Direction_In1, LOW); 
  // LOW Direction + 255 PWM Speed = reverse max speed
  // LOW Direction + 150 PWM Speed = reverse mid speed
  // LOW Direction + 1-99 PWM Speed = reverse invalid?? speed
  // LOW Direction + 0 PWM Speed = reverse 0 speed
  analogWrite(driveMotorA_PWMSpeed_In2, 0);
  digitalWrite(steerMotorB_In3, LOW); // IN3 = LOW; IN4 = HIGH --> RIGHT
  digitalWrite(steerMotorB_In4, LOW); // IN3 = HIGH; IN4 = LOW --> LEFT
  // IN3 = HIGH; IN4 = HIGH or IN3 = LOW; IN4 = LOW --> STRAIGHT
}
