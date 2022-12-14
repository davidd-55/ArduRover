const int driveMotorA_In1 = 3;  // PWM 3
const int driveMotorA_In2 = 5;  // PWM 5
const int steerMotorB_In3 = 9;  // PWM 9
const int steerMotorB_In4 = 10;  // PWM 10

const int LOW_SPEED = 100;
const int MID_SPEED = 175;
const int HIGH_SPEED = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(driveMotorA_In1, OUTPUT);
  pinMode(driveMotorA_In2, OUTPUT);
  pinMode(steerMotorB_In3, OUTPUT);
  pinMode(steerMotorB_In4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // sleep first 2s
  if (millis() <= 2000) {
    return;
  }

  // forward high speed 3s
  if (millis() > 2000 && millis() <= 5000) {
    forward(HIGH_SPEED);
  }

  // reverse mid speed 3s
  if (millis() > 5000 && millis() <= 8000) {
    reverse(MID_SPEED);
  }

  // left + forward
  if (millis() > 8000 && millis() <= 12000) {
    left();
    forward(MID_SPEED);
  }

  // right + reverse
  if (millis() > 12000 && millis() <= 16000) {
    right();
    reverse(MID_SPEED);
  }

  // reverse low
  if (millis() > 16000 && millis() <= 18000) {
    reverse(LOW_SPEED);
  }

  // forward low
  if (millis() > 18000 && millis() <= 20000) {
    forward(LOW_SPEED);
  }

  // end rotuine after 20s
  if (millis() > 20000) {
    shutdown();
  }
}

// Motor A IN1 SPEED + Motor A IN2 0 = FORWARD @SPEED
void forward(int speed) {
  analogWrite(driveMotorA_In1, speed);
  analogWrite(driveMotorA_In2, 0);
}

// Motor A IN1 0 + Motor A IN2 SPEED = REVERSE @SPEED
void reverse(int speed) {
  analogWrite(driveMotorA_In1, 0);
  analogWrite(driveMotorA_In2, speed);
}

// Motor B IN3 HIGH + Motor B IN4 0 = LEFT
void left() {
  analogWrite(steerMotorB_In3, HIGH); 
  analogWrite(steerMotorB_In4, 0); 
}

// Motor B IN3 0 + Motor B IN4 HIGH = RIGHT
void right() {
  analogWrite(steerMotorB_In3, 0); 
  analogWrite(steerMotorB_In4, HIGH); 
}

void shutdown() {
  analogWrite(driveMotorA_In1, 0);
  analogWrite(driveMotorA_In2, 0);
  analogWrite(steerMotorB_In3, 0); 
  analogWrite(steerMotorB_In4, 0); 
}
