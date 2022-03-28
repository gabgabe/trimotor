
#include <FlexyStepper.h> //test commit
#include <TeensyDMX.h>
#include "Ticker.h"
namespace teensydmx = ::qindesign::teensydmx;
teensydmx::Receiver dmxRx{Serial1};
const int MOTOR_STEP_PIN = 2;
const int MOTOR_DIRECTION_PIN = 3;
void debug();
void doStep();
Ticker timer1(debug, 100, 0, MILLIS);
Ticker timer2(doStep, 100, 0, MILLIS);
// Ticker timer3(checkMe, 23, 0, MILLIS);
FlexyStepper stepper;
uint16_t LV;
uint16_t AV;
uint16_t DV;
uint16_t CV;
uint16_t MV;

void setup()
{
  Serial.begin(115200);
  dmxRx.begin();
  timer1.start();
  timer2.start();
  stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
  stepper.setSpeedInStepsPerSecond(32 * 500);
  stepper.setAccelerationInStepsPerSecondPerSecond(32 * 2500);
}
void loop()
{
  timer1.update();
  timer2.update();
  AV = dmxRx.get16Bit(19);

  
  delay(10);
}
void debug()
{
  Serial.print("time: ");
  Serial.print(millis()/100);
  Serial.print("  DMX VAL: ");
  Serial.print(MV);
  Serial.print("  pos: ");
  Serial.println(stepper.getCurrentPositionInSteps());
}
void doStep()
{
  //  CV = LV + (DV / 24);
  //  map(AV, 0, 66535, 0, 12800);
  //  MV = map(CV, 0, 66535, 0, 6400);
  //  stepper.moveToPositionInSteps(AV);
  //  LV = CV;
  MV = map(AV, 0, 66535, 0, 6400);
  stepper.moveToPositionInSteps(MV);
}
