#include <FlexyStepper.h>

#include <cstring>

#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

// Create the DMX receiver on Serial1.
teensydmx::Receiver dmxRx{Serial1};

uint16_t lastValue = 0;
uint16_t Step = 0;
uint16_t position1 = 0;

int actualValue;
int oldValue = 0;
int steppe;
int posAct;
int pos;
FlexyStepper motor;       // STEP pin: 2, DIR pin: 3
//StepControl controller;    // Use default settings

void setup()
{
Serial.begin(115200);
    motor.connectToPins(2, 3);
    
    dmxRx.begin();
    oldValue = dmxRx.get16Bit(1);
    motor.setSpeedInStepsPerSecond(10000000000);
    motor.setAccelerationInStepsPerSecondPerSecond(20000);
}
void loop() {




Serial.println(dmxRx.get16Bit(1));
motor.setTargetPositionInSteps(dmxRx.get16Bit(1));

motor.processMovement();

}