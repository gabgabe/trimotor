#include <FlexyStepper.h>
#include <cstring>
#include <TeensyDMX.h>
#include "configurazione.h"
namespace teensydmx = ::qindesign::teensydmx;
void mot1endstop();
void mot2endstop();
void mot3endstop();

// Create the DMX receiver on Serial1.
teensydmx::Receiver dmxRx{Serial1};

FlexyStepper motor1; // STEP pin: 2, DIR pin: 3
FlexyStepper motor2; // STEP pin: 4, DIR pin: 5
FlexyStepper motor3; // STEP pin: 6, DIR pin: 7
void setup()
{


    pinMode(MOT_1_ENDSTOP_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MOT_1_ENDSTOP_PIN), mot1endstop, RISING);

    pinMode(MOT_2_ENDSTOP_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MOT_3_ENDSTOP_PIN), mot2endstop, RISING);
 
    pinMode(MOT_3_ENDSTOP_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MOT_3_ENDSTOP_PIN), mot3endstop, RISING);


    Serial.begin(115200);
    motor1.connectToPins(2, 3);
    motor2.connectToPins(4, 5);
    motor3.connectToPins(6, 7);
    dmxRx.begin();

    motor1.setSpeedInStepsPerSecond(10000000000);
    motor1.setAccelerationInStepsPerSecondPerSecond(20000);

    motor2.setSpeedInStepsPerSecond(10000000000);
    motor2.setAccelerationInStepsPerSecondPerSecond(20000);

    motor3.setSpeedInStepsPerSecond(10000000000);
    motor3.setAccelerationInStepsPerSecondPerSecond(20000);
}
void loop()
{

    // motor1
    Serial.println(dmxRx.get16Bit(1));
    motor1.setTargetPositionInSteps(dmxRx.get16Bit(1));

    // motor2
    Serial.println(dmxRx.get16Bit(3));
    motor2.setTargetPositionInSteps(dmxRx.get16Bit(3));

    // motor3
    Serial.println(dmxRx.get16Bit(5));
    motor3.setTargetPositionInSteps(dmxRx.get16Bit(5));

    motor1.processMovement();
    motor2.processMovement();
    motor3.processMovement();
}

void mot1endstop() {}
void mot2endstop() {}
void mot3endstop() {}