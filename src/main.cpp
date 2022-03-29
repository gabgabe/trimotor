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

boolean homing(FlexyStepper motorToHome, int endPin, int8_t direction)
{
    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED);
    while (!digitalRead(endPin)) // muovi verso endstop e testa while LOW
    {
        motorToHome.setTargetPositionInMillimeters(-MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }
    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL / 2);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED / 8);
    while (digitalRead(endPin)) // muovi verso su e testa while HIGH  && ((millis() - lastMillis) > 50)
    {                           //   Serial.println("mi allontando da endstop");
        motorToHome.setTargetPositionInMillimeters(MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }

    motorToHome.setCurrentPositionInMillimeters(0);
    Serial.println("Homing Completed");
    Serial.println("");

    return true;
}
void setup()
{
    dmxRx.begin();
    Serial.begin(115200);
    dmxRx.begin();
    motor1.connectToPins(MOT_1_STEP_PIN, MOT_1_DIR_PIN);
    motor2.connectToPins(MOT_2_STEP_PIN, MOT_2_DIR_PIN);
    motor3.connectToPins(MOT_3_STEP_PIN, MOT_3_DIR_PIN);
    motor1.setStepsPerMillimeter(MOT_1_STEPS_PER_MM);
    motor2.setStepsPerMillimeter(MOT_2_STEPS_PER_MM);
    motor3.setStepsPerMillimeter(MOT_3_STEPS_PER_MM);
    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED);
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED);
    motor1.setAccelerationInMillimetersPerSecondPerSecond(MOT_1_ACCEL);
    motor2.setAccelerationInMillimetersPerSecondPerSecond(MOT_2_ACCEL);
    motor3.setAccelerationInMillimetersPerSecondPerSecond(MOT_3_ACCEL);
    pinMode(ENDSTOP_UP_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_MID_UP_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_MID_DOWN_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_DOWN_PIN, INPUT_PULLUP);
    system_ready = false;
    motors_initialized = false;
    delay(1000);
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
    B = dmxRx.get16Bit(dmxStartChannel + 2);
    C = dmxRx.get16Bit(dmxStartChannel + 4);

#ifdef DEBUG_ON
    Serial.print("    CH1 :  ");
    Serial.print(A);
    Serial.print("    CH3 :  ");
    Serial.print(B);
    Serial.print("    CH5 :  ");
    Serial.println(C);
#endif

    A = map(A, 0, 65535, A_MIN, A_MAX);
    B = map(B, 0, 65535, B_MIN, B_MAX);
    C = map(C, 0, 65535, C_MIN, C_MAX);

    pA = constrain(A, A_MIN, pB);
    pB = constrain(B, pA, pC);
    pC = constrain(C, pB, C_MAX);

#ifdef DEBUG_ON
    Serial.print("    CH1 :  ");
    Serial.print(pA);
    Serial.print("    CH3 :  ");
    Serial.print(pB);
    Serial.print("    CH5 :  ");
    Serial.println(pC);
#endif
}
void systemInitialization()
{
    delay(1000);
    if (homing(motor1, ENDSTOP_DOWN_PIN, MOT_1_DIRECTION))
    {
        delay(500);
        if (homing(motor2, ENDSTOP_MID_DOWN_PIN, MOT_2_DIRECTION))
        {
            delay(500);
            if (homing(motor3, ENDSTOP_MID_UP_PIN, MOT_3_DIRECTION))
            {
                motors_initialized = true;
                delay(500);
            }
        }
    }
    else
        break;
}