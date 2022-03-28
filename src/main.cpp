#include <FlexyStepper.h>
#include <cstring
#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

// Create the DMX receiver on Serial1.
teensydmx::Receiver dmxRx{Serial1};

FlexyStepper motor1;       // STEP pin: 2, DIR pin: 3
FlexyStepper motor2;       // STEP pin: 4, DIR pin: 5
FlexyStepper motor3;       // STEP pin: 6, DIR pin: 7
void setup()
{
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
void loop() {



//motor1
Serial.println(dmxRx.get16Bit(1));
motor1.setTargetPositionInSteps(dmxRx.get16Bit(1));

//motor2
Serial.println(dmxRx.get16Bit(3));
motor2.setTargetPositionInSteps(dmxRx.get16Bit(3));

//motor3
Serial.println(dmxRx.get16Bit(5));
motor3.setTargetPositionInSteps(dmxRx.get16Bit(5));

motor1.processMovement();
motor2.processMovement();
motor3.processMovement();

}
