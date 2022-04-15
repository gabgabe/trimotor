#include <arduino.h>
void setup(){
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    Serial.begin(112500);
}

void loop(){
    if(!digitalRead(4)){
    Serial.println("4");
    }

    if(!digitalRead(5)){
    Serial.println("5");
    }
    if(!digitalRead(6)){
    Serial.println("6");
    }
    if(!digitalRead(4)){
    Serial.println("4");
    }
    

}