
#include <FlexyStepper.h>
#include <cstring>
#include <TeensyDMX.h>
#include "configurazione.h"

void systemInitialization(); // esegue l'homing e calcola le distanze massime
void run();                  // stato di run su dmx, equivale all'on-air di uno studio
void moveMotors();           // processa i movimenti dei 3 motori
void processMeasures();      // calcola lo spazio possibile e


boolean debounce(int btnPin, FlexyStepper motorInDebounce)
{
    motorInDebounce.processMovement();
    int reading = digitalRead(btnPin);
    if (reading != lastButtonState)
    {
        motorInDebounce.processMovement();
        lastDebounceTime = millis();
    }
    motorInDebounce.processMovement();
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        motorInDebounce.processMovement();
        if (reading != buttonState)
        {
            motorInDebounce.processMovement();
            buttonState = reading;
            if (buttonState == HIGH)
            {
                return true;
            }
        }
    }
    motorInDebounce.processMovement();
    lastButtonState = reading;
    return false;
}
boolean homing(FlexyStepper motorToHome, int endPin, int8_t direction)
{
    motorToHome.setStepsPerMillimeter(MOT_1_STEPS_PER_MM);

    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED);
    while (!debounce(endPin, motorToHome)) // muovi verso endstop e testa while LOW
    {
        motorToHome.setTargetPositionInMillimeters(-MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }
    motorToHome.setStepsPerMillimeter(MOT_1_STEPS_PER_MM);

    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL / 2);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED / 8);
    while (digitalRead(endPin)) // muovi verso su e testa while HIGH  && ((millis() - lastMillis) > 50)debounce(endPin, motorToHome)
    {                        //   Serial.println("mi allontando da endstop");
        motorToHome.setTargetPositionInMillimeters(MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }
    motorToHome.setCurrentPositionInMillimeters(0);
    Serial.println("Homing Completed");
    Serial.println("");
    delay(1000);
    return true;
}

void setup()
{
    // Serial.begin(115200);
    dmxRx.begin();
    motor1.connectToPins(MOT_1_STEP_PIN, MOT_1_DIR_PIN);
    motor2.connectToPins(MOT_2_STEP_PIN, MOT_2_DIR_PIN);
    motor3.connectToPins(MOT_3_STEP_PIN, MOT_3_DIR_PIN);
    motor1.setStepsPerMillimeter(MOT_1_STEPS_PER_MM);
    motor2.setStepsPerMillimeter(MOT_2_STEPS_PER_MM);
    motor3.setStepsPerMillimeter(MOT_3_STEPS_PER_MM);
    pinMode(ENDSTOP_UP_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_MID_UP_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_MID_DOWN_PIN, INPUT_PULLUP);
    pinMode(ENDSTOP_DOWN_PIN, INPUT_PULLUP);
    pinMode(24, OUTPUT); // necessari per abilitare lettura dmx
    digitalWrite(24, LOW);
    pinMode(MOT_1_EN_PIN, OUTPUT);
    digitalWrite(MOT_1_EN_PIN, LOW);
    systemInitialization();
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
    motor1.setTargetPositionInMillimeters(pA * MOT_1_DIRECTION);
    motor2.setTargetPositionInMillimeters(pB * MOT_2_DIRECTION);
    motor3.setTargetPositionInMillimeters(pC * MOT_3_DIRECTION);
    motor1.processMovement();
    motor2.processMovement();
    motor3.processMovement();
}
void processMeasures()
{
    A = dmxRx.get16Bit(dmxStartChannel);
    B = dmxRx.get16Bit(dmxStartChannel + 2);
    C = dmxRx.get16Bit(dmxStartChannel + 4);
    A = map(A, 0, 65535, A_MIN, A_MAX);
    B = map(B, 0, 65535, B_MIN, B_MAX);
    C = map(C, 0, 65535, C_MIN, C_MAX);
    pA = constrain(A, A_MIN, pB);
    pB = constrain(B, pA, pC);
    pC = constrain(C, pB, C_MAX);
}
void systemInitialization()
{
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
    delay(1000);

    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED / 2); // reimposto i valori di default dopo homing
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED / 2);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED / 2);
    motor1.setAccelerationInMillimetersPerSecondPerSecond(MOT_1_ACCEL);
    motor2.setAccelerationInMillimetersPerSecondPerSecond(MOT_2_ACCEL);
    motor3.setAccelerationInMillimetersPerSecondPerSecond(MOT_3_ACCEL);
    motor1.moveToPositionInMillimeters(A_MAX / 2 * MOT_1_DIRECTION);
    motor2.moveToPositionInMillimeters(B_MAX / 2 * MOT_2_DIRECTION);
    motor3.moveToPositionInMillimeters(C_MAX / 2 * MOT_3_DIRECTION);
    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED); // reimposto i valori di default dopo homing
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED);
}
void 