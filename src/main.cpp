#include <FlexyStepper.h>

#include <cstring>

#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

// Create the DMX receiver on Serial1.
teensydmx::Receiver dmxRx{Serial1};

FlexyStepper motor1;       // STEP pin: 2, DIR pin: 3
FlexyStepper motor2;       // STEP pin: 4, DIR pin: 5
FlexyStepper motor3;       // STEP pin: 6, DIR pin: 7

const int pin8 = 8;

boolean up;
boolean down;

boolean homingFinished = false;

boolean home;
long initial_homing = -1;
int move_finished = 1;

boolean homing(FlexyStepper motorToHome){

  motorToHome.setAccelerationInStepsPerSecondPerSecond(100000);
  motorToHome.setSpeedInStepsPerSecond(1000);
  while(digitalRead(pin8)){
    motorToHome.moveRelativeInSteps(initial_homing);
    initial_homing--;
    motorToHome.processMovement();
    delay(5);
  }
  motorToHome.setCurrentPositionInSteps(0);
  initial_homing = 1;

  while (!digitalRead(pin8)) { // Make the Stepper move CW until the switch is deactivated
    motorToHome.setAccelerationInStepsPerSecondPerSecond(10000);
    motorToHome.setSpeedInStepsPerSecond(1000);
    motorToHome.moveRelativeInSteps(initial_homing);  
    initial_homing++;
    motorToHome.processMovement();
    delay(5);  
  }

  motorToHome.setCurrentPositionInSteps(0);
  return true;
  
  Serial.println("Homing Completed");
  Serial.println("");
}

void setup()
{
    Serial.begin(115200);

    pinMode(pin8, INPUT_PULLUP);

    motor1.connectToPins(2, 3);
    motor2.connectToPins(4, 5);
    motor3.connectToPins(6, 7);
    dmxRx.begin();
    
    homingFinished = homing(motor1);

    motor1.setSpeedInStepsPerSecond(10000000000);
    motor1.setAccelerationInStepsPerSecondPerSecond(20000);

    motor2.setSpeedInStepsPerSecond(10000000000);
    motor2.setAccelerationInStepsPerSecondPerSecond(20000);

    motor3.setSpeedInStepsPerSecond(10000000000);
    motor3.setAccelerationInStepsPerSecondPerSecond(20000);

    
}

void loop() {

//motor1.setCurrentPositionInMillimeters(325);

//motor1
Serial.println(dmxRx.get16Bit(1));
motor1.setTargetPositionInSteps(dmxRx.get16Bit(1)/5);

//motor2
Serial.println(dmxRx.get16Bit(3));
motor2.setTargetPositionInSteps(dmxRx.get16Bit(3));

//motor3
Serial.println(dmxRx.get16Bit(5));
motor3.setTargetPositionInSteps(dmxRx.get16Bit(5));


if(homingFinished){
  motor1.processMovement();
}
motor2.processMovement();
motor3.processMovement();


}


