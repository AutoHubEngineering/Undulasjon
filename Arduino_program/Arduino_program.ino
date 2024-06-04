#include <Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#define PI 3.14159265358979323
#define NumServos 6           //The number of servos used
//Tweaking constants
#define Hz 0.5                //The frequency of the wave 
#define PhaseShift PI / 2     //Shifting the phase with the distance between each arm/servo
#define Amplitude 40          //The ampitude in degrees

float CurrentRad;             //The radian posistion of the first servo

Adafruit_INA219 ina219;
Servo servo[NumServos];
int servoPin[NumServos] = {2, 3, 4, 5, 6, 7};       //The pin to the servos on the "Arduino Mega 2560". The first servo is servo[0].

int servoAngleCalc(int servoNumber){
  float servoRad = CurrentRad + (servoNumber * PhaseShift);     //Calculate the radian position to the servo
  double armPos = sin(servoRad);                                //Calculates the arm position. How far the arm are from the equalibrium line. 
  //Serial.println("ArmPos: " + String(armPos));
  int armPosInt = armPos * 10000;
  //Serial.println("ArmPosInt: " + String(armPosInt));
  int servoAngle = map(armPosInt, -10000, 10000, -Amplitude, Amplitude);   //Mapping from the sin value to the rigth degree for the servo
  //int servoAngle = (int)((armPos + 1) * (Amplitude / 2.0)); 
  //Serial.println("Servo angle: " + String(servoAngle));
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
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  CurrentRad = 2 * PI * millis() * Hz * 0.001;      //Sinus wave generator

  for(int i = 0; i < NumServos; i++){
    servo[i].write(servoAngleCalc(i)+90);
    //Serial.println("write to a servo");
  }

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(10);
}
