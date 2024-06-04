#include <Servo.h>

#define PI 3.14159265358979323
#define NumServos 6             //The number of servos used
#define PhaseShift PI / 2       //Shifting the phase with the distance between each arm/servo
#define StepTime 8000           //The time of each step in mS
#define Steps 10                //The number of steps that will be executed 

//Tweaking constants
bool HzTweaking = false;        //True when Hz tweaking is needed
#define HzStart 0.4             //The starting frequency of the wave 
#define HzStepIncrease 0.02     //The Hz increase for each step
bool AmplitudeTweaking = false; //True when Amplitude tweaking is needed
#define AmplitudeStart 30       //The starting ampitude in degrees
#define AmplitudeStepIncrease 1 //The Hz increase for each step


//Global variables
float CurrentRad;               //The radian posistion of the first servo
float Hz = HzStart;
float Amplitude = AmplitudeStart;
unsigned long StepTimer;
int StepCounter = 1;


//Setting up the servos 
Servo servo[NumServos];
int servoPin[NumServos] = {2, 3, 4, 5, 6, 7};       //The pin to the servos on the "Arduino Mega 2560". The first servo is servo[0].


//The servo angle calculator function. Returns the next angle to the servo
int servoAngleCalc(int servoNumber){
  float servoRad = CurrentRad + (servoNumber * PhaseShift);     //Calculate the radian position to the servo
  double armPos = sin(servoRad);                                //Calculates the arm position. How far the arm are from the equalibrium line. 
  //Serial.println("ArmPos: " + String(armPos));
  int armPosInt = armPos * 10000;                               //Turn the armPos into a int, but multipy it to get good reselution 
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

  //Starting the step timer and writes the first step
  StepTimer = millis() + StepTime;
  Serial.print("The" + String(StepCounter) + "th step:   Hz: " + String(Hz)+"   Amplitude: " + String(Amplitude));
}

void loop() {
  CurrentRad = 2 * PI * millis() * Hz * 0.001;      //Sinus wave generator

  if(millis() > StepTimer){
    StepCounter++;
    if(StepCounter > Steps){
      Serial.println("Sequence tweaking test finished...");
      delay(999999999);
    }
    Serial.print("The" + String(StepCounter) + "th step:  ");
    StepTimer = millis() + StepTime;
    if(HzTweaking){
      Hz = Hz + HzStepIncrease;
      Serial.print("Hz: " + String(Hz));
    }
    if(AmplitudeTweaking){
      Amplitude = Amplitude + AmplitudeStepIncrease;
      Serial.print("Amplitude: " + String(Amplitude));
    }
    Serial.println(" ");
  }

  for(int i = 0; i < NumServos; i++){
    servo[i].write(servoAngleCalc(i)+90);
    //Serial.println("write to a servo");
  }
  
  delay(10);
}

