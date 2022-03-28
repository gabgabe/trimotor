#include <FlexyStepper.h>
#include <cstring>
#include <TeensyDMX.h>
#include "configurazione.h"
namespace teensydmx = ::qindesign::teensydmx;
void mot1endstop();
void mot2endstop();
void mot3endstop();
void run();

// Create the DMX receiver on Serial1.
teensydmx::Receiver dmxRx{Serial1};

FlexyStepper motor1; // STEP pin: 2, DIR pin: 3
FlexyStepper motor2; // STEP pin: 4, DIR pin: 5
FlexyStepper motor3; // STEP pin: 6, DIR pin: 7
void setup()
{

    Serial.begin(115200);
    motor1.connectToPins(MOT_1_STEP_PIN, MOT_1_DIR_PIN);
    motor2.connectToPins(MOT_2_STEP_PIN, MOT_2_DIR_PIN);
    motor3.connectToPins(MOT_3_STEP_PIN, MOT_3_DIR_PIN);
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