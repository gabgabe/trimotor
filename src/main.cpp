<<<<<<< HEAD
=======
#include <FlexyStepper.h>
#include <cstring>
#include <TeensyDMX.h>
#include "configurazione.h"
namespace teensydmx = ::qindesign::teensydmx;
void systemInitialization(); // esegue l'homing e calcola le distanze massime
void mot1endstop();
void mot2endstop();
void mot3endstop();
void mot4endstop();
void run();             // stato di run su dmx, equivale all'on-air di uno studio
void moveMotors();      // processa i movimenti dei 3 motori
void processMeasures(); // calcola lo spazio possibile e
#ifdef PROTOTYPE
teensydmx::Receiver dmxRx{Serial3}; // Create the DMX receiver on Serial1.
#endif
#ifdef SVILUPPO
teensydmx::Receiver dmxRx{Serial1}; // Create the DMX receiver on Serial1.
#endif

FlexyStepper motor1;
FlexyStepper motor2;
FlexyStepper motor3;

boolean homing(FlexyStepper motorToHome, int endPin){

  motorToHome.setAccelerationInStepsPerSecondPerSecond(100000);
  motorToHome.setSpeedInStepsPerSecond(1000);
  while(digitalRead(endPin)){
    motorToHome.moveRelativeInSteps(initial_homing);
    initial_homing--;
    motorToHome.processMovement();
    delay(5);
  }
  motorToHome.setCurrentPositionInSteps(0);
  initial_homing = 1;

  while (!digitalRead(endPin)) { // Make the Stepper move CW until the switch is deactivated
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
    motor1.connectToPins(MOT_1_STEP_PIN, MOT_1_DIR_PIN);
    motor2.connectToPins(MOT_2_STEP_PIN, MOT_2_DIR_PIN);
    motor3.connectToPins(MOT_3_STEP_PIN, MOT_3_DIR_PIN);
    dmxRx.begin();
    motor1.setStepsPerMillimeter(MOT_1_STEPS_PER_MM);
    motor2.setStepsPerMillimeter(MOT_2_STEPS_PER_MM);
    motor3.setStepsPerMillimeter(MOT_3_STEPS_PER_MM);

    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED);
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED);

    motor1.setAccelerationInMillimetersPerSecondPerSecond(MOT_1_ACCEL);
    motor2.setAccelerationInMillimetersPerSecondPerSecond(MOT_2_ACCEL);
    motor3.setAccelerationInMillimetersPerSecondPerSecond(MOT_3_ACCEL);
    systemInitialization();
}
void loop()
{
    run();
}

void run()
{
    processMeasures();
    moveMotors();
}
void moveMotors()
{
    motor1.setTargetPositionInMillimeters(pA);
    motor2.setTargetPositionInMillimeters(pB);
    motor3.setTargetPositionInMillimeters(pC);
    motor1.processMovement();
    motor2.processMovement();
    motor3.processMovement();
}
void processMeasures()
{
    A = dmxRx.get16Bit(dmxStartChannel);
    //    Serial.print("    CH1 :  ");
    //    Serial.print(A);
    B = dmxRx.get16Bit(dmxStartChannel + 2);
    //    Serial.print("    CH3 :  ");
    //    Serial.print(B);
    C = dmxRx.get16Bit(dmxStartChannel + 4);
    //    Serial.print("    CH5 :  ");
    //    Serial.println(C);

    A = map(A, 0, 65535, A_MIN, A_MAX);
    B = map(B, 0, 65535, B_MIN, B_MAX);
    C = map(C, 0, 65535, C_MIN, C_MAX);

    pA = constrain(A, A_MIN, pB);
    pB = constrain(B, pA, pC);
    pC = constrain(C, pB, C_MAX);
/*
    Serial.print("    CH1 :  ");
    Serial.print(pA);
    Serial.print("    CH3 :  ");
    Serial.print(pB);
    Serial.print("    CH5 :  ");
    Serial.println(pC);
    */
}
void mot1endstop() {}
void mot2endstop() {}
void mot3endstop() {}
void mot4endstop() {}
void systemInitialization() {}
>>>>>>> gab
