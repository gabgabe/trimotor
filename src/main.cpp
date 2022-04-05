
#include <FlexyStepper.h>
#include <cstring>
#include <TeensyDMX.h>
#include "configurazione.h"
//#include <Ticker.h>

void systemInitialization(); // esegue l'homing e calcola le distanze massime
void run();                  // stato di run su dmx, equivale all'on-air di uno studio
void moveMotors();           // processa i movimenti dei 3 motori
void processMeasures();      // calcola lo spazio possibile e
void processMeasures2();     // calcola lo spazio possibile e
void getDmx();
bool getDmxPresence();
void checkCollision();
// Ticker tickCheckCollision(checkCollision, 100, 0, MILLIS);

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
boolean homing(FlexyStepper motorToHome, int8_t enPin, int8_t endPin, int8_t direction)
{
    motorToHome.setCurrentPositionInSteps(0);
    delay(100);
    pinMode(enPin, OUTPUT); // ENABLE MOTOR
    digitalWrite(enPin, LOW);
    delay(500);
    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED / 2);
    while (!debounce(endPin, motorToHome)) // muovi verso endstop e testa while LOW
    {
        motorToHome.setTargetPositionInMillimeters(-MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }
    motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL / 2);
    motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED / 4);
    while (digitalRead(endPin))
    {
        motorToHome.setTargetPositionInMillimeters(MOT_HOMING_MAX_DISTANCE * direction);
        motorToHome.processMovement();
    }
    delay(500);
    motorToHome.setCurrentPositionInSteps(0);
    if (HOMING_OFFSET > 0)
    {
        motorToHome.setAccelerationInMillimetersPerSecondPerSecond(MOT_HOMING_ACCEL);
        motorToHome.setSpeedInMillimetersPerSecond(MOT_HOMING_SPEED / 2);
        motorToHome.moveToPositionInMillimeters(HOMING_OFFSET * direction);
        motorToHome.setCurrentPositionInSteps(0);
    }
    delay(500);
    return true;
}

void setup()
{
    Serial.begin(115200);
    pinMode(24, OUTPUT); // necessari per abilitare lettura dmx
    digitalWrite(24, LOW);
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
    pinMode(MOT_1_EN_PIN, OUTPUT); // ENABLE MOTOR 1
    pinMode(MOT_2_EN_PIN, OUTPUT); // ENABLE MOTOR 2
    pinMode(MOT_3_EN_PIN, OUTPUT); // ENABLE MOTOR 3
    digitalWrite(MOT_1_EN_PIN, HIGH);
    digitalWrite(MOT_2_EN_PIN, HIGH);
    digitalWrite(MOT_3_EN_PIN, HIGH);
    systemInitialization();
    system_ready = false;
    motors_initialized = false;

markRepeat:
    if (!getDmxPresence())
    {
        goto markRepeat;
    }
}
void loop()
{
    run();
    // getDmxPresence();
}
void run()
{
    getDmx();
    processMeasures(); // processMeasures(motore1, motore2, motore3); || processMeasures(motore1, motore2, motore3, cuscinetto);
    moveMotors();
    checkCollision();

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
    A = map(A, 0, 65535, A_MIN, A_MAX); // canale dmx in millimetri
    B = map(B, 0, 65535, B_MIN, B_MAX); // canale dmx in millimetri
    C = map(C, 0, 65535, C_MIN, C_MAX); // canale dmx in millimetri
    pA = constrain(A, A_MIN, pB);
    pB = constrain(B, pA - 1, pC + 1);
    pC = constrain(C, pB, C_MAX);
    
}
void systemInitialization()
{

    A = 0;  // variabile interna
    B = 0;  // variabile interna
    C = 0;  // variabile interna
    pA = 0; // variabile interna
    pB = 0; // variabile interna
    pC = 0; // variabile interna
    A_MIN = 0;
    B_MIN = 0;
    C_MIN = 0;
    A_MAX = 1800;         // altezza massima motore A
    B_MAX = 1800;         // altezza massima motore B
    C_MAX = 1800;         // altezza massima motore C
    UNDER_A_OFFSET = 0;   // gap aggiuntivo tra carrello inferiore e pavimento
    BETWEEN_B_OFFSET = 0; // gap aggiuntivo tra carrello centrale e i due esterni
    OVER_C_OFFSET = 0;    // gap aggiuntivo tra carrello superiore e soffitto
    CUSCINETTO = 0;       // come i tre sopra, misura unificata

    if (homing(motor1, MOT_1_EN_PIN, ENDSTOP_DOWN_PIN, MOT_1_DIRECTION))
    {
        if (homing(motor2, MOT_2_EN_PIN, ENDSTOP_MID_DOWN_PIN, MOT_2_DIRECTION))
        {
            if (homing(motor3, MOT_3_EN_PIN, ENDSTOP_MID_UP_PIN, MOT_3_DIRECTION))
            {
                motors_initialized = true;
            }
        }
    }

    delay(500);

    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED / 2);
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED / 2);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED / 2);
    motor1.setAccelerationInMillimetersPerSecondPerSecond(MOT_1_ACCEL);
    motor2.setAccelerationInMillimetersPerSecondPerSecond(MOT_2_ACCEL);
    motor3.setAccelerationInMillimetersPerSecondPerSecond(MOT_3_ACCEL);

    motor3.setCurrentPositionInSteps(0);
    motor3.moveToPositionInMillimeters(C_MAX / 2 * MOT_3_DIRECTION);

    if (motor3.motionComplete())
    {
    }
    // Serial.println("motor 3 moving, motor 2 waiting");
    else
        motor2.setCurrentPositionInSteps(0);
    motor2.moveToPositionInMillimeters(B_MAX / 2 * MOT_2_DIRECTION);

    if (motor2.motionComplete())
    {
    }
    // Serial.println("motor 3 moving, motor 2 waiting");
    else
        motor1.setCurrentPositionInSteps(0);
    motor1.moveToPositionInMillimeters(A_MAX / 2 * MOT_1_DIRECTION);
    if (motor1.motionComplete())
    {
    }

    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED);
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED);
    delay(1000);
}
void getDmx()
{
    A = dmxRx.get16Bit(dmxStartChannel);
    B = dmxRx.get16Bit(dmxStartChannel + 2);
    C = dmxRx.get16Bit(dmxStartChannel + 4);
    D = dmxRx.get(8);
}
bool getDmxPresence()
{
    if (dmxRx.connected())
    {
        Serial.println("legge");
        return true;
    }
    else
        return false;
    Serial.println("      non legge");
}
void testMotors()
{

    motor1.setSpeedInMillimetersPerSecond(MOT_1_SPEED / 2); // reimposto i valori di default dopo homing
    motor2.setSpeedInMillimetersPerSecond(MOT_2_SPEED / 2);
    motor3.setSpeedInMillimetersPerSecond(MOT_3_SPEED / 2);
    motor1.setAccelerationInMillimetersPerSecondPerSecond(MOT_1_ACCEL);
    motor2.setAccelerationInMillimetersPerSecondPerSecond(MOT_2_ACCEL);
    motor3.setAccelerationInMillimetersPerSecondPerSecond(MOT_3_ACCEL);

    delay(1000);
    motor1.moveToPositionInMillimeters(20 * MOT_1_DIRECTION);
    motor1.moveToPositionInMillimeters(-20 * MOT_1_DIRECTION);
    delay(500);
    motor2.moveToPositionInMillimeters(20 * MOT_2_DIRECTION);
    motor2.moveToPositionInMillimeters(-20 * MOT_2_DIRECTION);
    delay(500);
    motor3.moveToPositionInMillimeters(20 * MOT_3_DIRECTION);
    motor3.moveToPositionInMillimeters(-20 * MOT_3_DIRECTION);
    delay(500);
}
void checkCollision()
{
    if (digitalRead(ENDSTOP_DOWN_PIN) || digitalRead(ENDSTOP_MID_DOWN_PIN) || digitalRead(ENDSTOP_MID_UP_PIN) || digitalRead(ENDSTOP_UP_PIN))
    {
        digitalWrite(MOT_1_EN_PIN, HIGH);
        digitalWrite(MOT_2_EN_PIN, HIGH);
        digitalWrite(MOT_3_EN_PIN, HIGH);
        Serial.println("BREAK||||||||||||||||||||||||||motor 3:||||||||||||||||||||||||||");
        motor3.debug();
        Serial.println("BREAK||||||||||||||||||||||||||motor 2:||||||||||||||||||||||||||");
        motor2.debug();
        Serial.println("BREAK||||||||||||||||||||||||||motor 1:||||||||||||||||||||||||||");
        motor1.debug();
        motor3.libraryReset();
        motor2.libraryReset();
        motor1.libraryReset();
        delay(2000);
        systemInitialization();
    }
}
void processMeasures2()
{
    A = map(A, 0, 65535, A_MIN, A_MAX); // canale dmx in millimetri
    B = map(B, 0, 65535, B_MIN, B_MAX); // canale dmx in millimetri
    C = map(C, 0, 65535, C_MIN, C_MAX); // canale dmx in millimetri
    pA = constrain(A, A_MIN, motor2.getCurrentPositionInMillimeters()-50);
    pB = constrain(B, A_MIN+50, C_MAX-50);
    pC = constrain(C, motor2.getCurrentPositionInMillimeters()+50, C_MAX);
    
/*
Serial.print("a:");
Serial.print(motor1.getCurrentPositionInMillimeters());
Serial.print("  b: ");
Serial.print(motor2.getCurrentPositionInMillimeters());
Serial.print("  c: ");
Serial.println(motor3.getCurrentPositionInMillimeters());
*/
}
