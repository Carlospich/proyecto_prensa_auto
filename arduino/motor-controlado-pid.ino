/*
Autor: Luis Ignacio Forero Molina
Este codigo esta basado en el algoritmo creado por  https://www.youtube.com/watch?v=bsB5shP3vls&ab_channel=TheBoredRobot
*/

//Define pin numbers for motor 
#define DIR1 4
#define PWM1 5

//Define pin numbers for encoders
#define encoderPinA 2
#define encoderPinB 3

//Variable for encoder counts
volatile long encoderCount = 0;

//Variables for PID Control
long previousTime = 0;
float ePrevious = 0;
float eIntegral = 0;

void setup() {
  Serial.begin(9600);

  //Set pin modes
  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoder, RISING);
}

void loop() {
  //Setpoint
  int target = 1000;

  //PID gains and computation
  float kp = 0.0;
  float kd = 0.0;
  float ki = 0.0;
  float u = pidController(target, kp, kd, ki);

  //Control motor 
  moveMotor(DIR1, PWM1, u);

  //Print statements for debugging

  Serial.print(target);
  Serial.print(", ");
  Serial.println(encoderCount);

}
void handleEncoder() {
  if (digitalRead(encoderPinA) > digitalRead(encoderPinB)){
    encoderCount++;
  }
  else{
    encoderCount--;
  }
}

//Function to move motor

void moveMotor(int dirPin, int pwmPin, float u) {
  //Maximum motor speed
  float speed = fabs(u);
  if (speed > 255){
    speed = 255;
  }
  //Set the direction
  int direction = 1;
  if(u < 0){
    direction= 0;
  }
  //Control the motor 
  digitalWrite(dirPin, direction);
  analogWrite(pwmPin, speed);
}

float pidController(int target, float kp, float kd, float ki) {
  //Measure the time elapsed since the last iteration
  long currentTime = micros();
  float deltaT = ((float)(currentTime - previousTime)) / 1.0e6;

  //Compute the error, derivative, and integral
  int e = encoderCount - target;
  float eDerivative = (e - ePrevious) / deltaT;
  eIntegral = eIntegral + e * deltaT;

  //Compute the PID control signal
  float u =(kp * e) + (kd * eDerivative) + (ki * eIntegral);

  //Update variables for the next iteration
  previousTime = currentTime;
  ePrevious = e;

  return u;

}
