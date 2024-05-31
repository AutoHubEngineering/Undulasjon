#include <Servo.h>

#define PI 3.14159265358979323
#define NumServos 6           //The number of servos used
//Tweaking constants
#define Hz 0.5                //The frequency of the wave 
#define PhaseShift PI / 2     //Shifting the phase with the distance between each arm/servo
#define Amplitude 40          //The ampitude in degrees

float CurrentRad;             //The radian posistion of the first servo


Servo servo[NumServos];
int servoPin[NumServos] = {2, 3, 4, 5, 6, 7};       //The pin to the servos on the "Arduino Mega 2560". The first servo is servo[0].

int servoAngleCalc(int servoNumber){
  float servoRad = CurrentRad + (servoNumber * PhaseShift);     //Calculate the radian position to the servo
  double armPos = sin(servoRad);                                //Calculates the arm position. How far the arm are from the equalibrium line. 
  int servoAngle = map(armPos, -1, 1, -Amplitude, Amplitude);   //Mapping from the sin value to the rigth degree for the servo
  return servoAngle;
}





void setup() {
  Serial.begin(115200);
  Serial.println("Starting up the system...");

  //Attach pins to servos
  for(int i = 0; i < NumServos; i++){
    servo[i].attach(servoPin[i]);
  }


}

void loop() {
  CurrentRad = 2 * PI * millis() * Hz * 0.001;      //Sinus wave generator

  for(int i = 0; i < NumServos; i++){
    servo[i].write(servoAngleCalc(i));
  }

  delay(10);
}
