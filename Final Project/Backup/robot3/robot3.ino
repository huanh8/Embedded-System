#include <IRremote.h>  // Include the IRremote library

// IR remote control pin
const int IR_PIN = 4;

// IR remote control codes
const unsigned long FORWARD_CODE = 3877175040;
const unsigned long BACKWARD_CODE = 2907897600;
const unsigned long LEFT_CODE = 4144561920;
const unsigned long RIGHT_CODE = 2774204160;
const unsigned long STOP_CODE = 3810328320;

// Motor control pins
const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;
const int BIN1 = 8;
const int BIN2 = 9;
const int PWMB = 10;

int switchPin = 7;             //switch to turn the robot on and off

// Robot behavior variables
int backupTime = 100;
int turnTime = 200;
int SPPED = 255;


// IR receiver instance
IRrecv irReceiver(IR_PIN);

// Robot states
enum RobotState {
  IDLE,
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT
};

RobotState currentState = IDLE;

void setup()
{

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  irReceiver.enableIRIn();

  Serial.begin(9600);
  Serial.println("To infinity and beyond!");
}

void loop()
{
  if (digitalRead(switchPin) == LOW) 
  {  currentState = IDLE;
      Serial.println("switch off");
    return;
  }
    if (irReceiver.decode()) {
      unsigned long code = irReceiver.decodedIRData.decodedRawData;
      Serial.println(code);
      switch (code) {
        case FORWARD_CODE:
          currentState = FORWARD;
          break;
        case BACKWARD_CODE:
          currentState = BACKWARD;
          break;
        case LEFT_CODE:
          currentState = TURN_LEFT;
          break;
        case RIGHT_CODE:
          currentState = TURN_RIGHT;
          break;
        case STOP_CODE:
          currentState = IDLE;
          break;
      }

      irReceiver.resume();
    }

  executeState(currentState);

  delay(50);
}

void executeState(RobotState state)
{
  switch (state) {
    case IDLE:
      stopRobot();
      break;
    case FORWARD:
      driveForward();
      break;
    case BACKWARD:
      driveBackward();
      break;
    case TURN_LEFT:
      turnLeft();
      break;
    case TURN_RIGHT:
      turnRight();
      break;
  }
}


void driveForward()
{
  rightMotor(SPPED);
  leftMotor(SPPED);
}

void driveBackward()
{
  rightMotor(-SPPED);
  leftMotor(-SPPED);
  delay(backupTime);
  stopRobot();
}

void turnLeft()
{
  rightMotor(SPPED);
  leftMotor(-SPPED);
  delay(turnTime);
}

void turnRight()
{
  rightMotor(-SPPED);
  leftMotor(SPPED);
  delay(turnTime);
}

void stopRobot()
{
  rightMotor(0);
  leftMotor(0);
}

void rightMotor(int motorSPPED)
{
  if (motorSPPED > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else if (motorSPPED < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }

  analogWrite(PWMA, abs(motorSPPED));
}

void leftMotor(int motorSPPED)
{
  if (motorSPPED > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else if (motorSPPED < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }

  analogWrite(PWMB, abs(motorSPPED));
}

