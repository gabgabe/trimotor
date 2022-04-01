#define MAINBOARD // PROTOTYPE per pcb Delio || SVILUPPO per default
//#define DEBUG_ON
#define ONEMOTOR

#ifdef MAINBOARD
// configurazione IO
#define MOT_1_STEP_PIN 37
#define MOT_1_DIR_PIN 38
#define MOT_1_EN_PIN 36

#define MOT_2_STEP_PIN 16
#define MOT_2_DIR_PIN 17
#define MOT_2_EN_PIN 18

#define MOT_3_STEP_PIN 29
#define MOT_3_DIR_PIN 30
#define MOT_3_EN_PIN 28

#define ENDSTOP_UP_PIN 1
#define ENDSTOP_MID_UP_PIN 2
#define ENDSTOP_MID_DOWN_PIN 3
#define ENDSTOP_DOWN_PIN 4
#endif
#ifdef PROTO
// configurazione IO
#define MOT_1_STEP_PIN 2
#define MOT_1_DIR_PIN 3
#define MOT_1_EN_PIN 4

#define MOT_2_STEP_PIN 5
#define MOT_2_DIR_PIN 6
#define MOT_2_EN_PIN 7

#define MOT_3_STEP_PIN 8
#define MOT_3_DIR_PIN 9
#define MOT_3_EN_PIN 10

#define ENDSTOP_UP_PIN 12
#define ENDSTOP_MID_UP_PIN 13
#define ENDSTOP_MID_DOWN_PIN 14
#define ENDSTOP_DOWN_PIN 15
#endif

// configurazione DMX
#define dmxStartChannel 1

// configurazione stepper
#define MOT_1_STEPS_PER_MM 80.0 // step/mm
#define MOT_2_STEPS_PER_MM 80.0 // step/mm
#define MOT_3_STEPS_PER_MM 80.0 // step/mm
#define MOT_1_SPEED 500.0      // mm/s
#define MOT_2_SPEED 500.0      // mm/s
#define MOT_3_SPEED 500.0      // mm/s
#define MOT_HOMING_SPEED 50.0      // mm/s

#define MOT_1_ACCEL 500.0       // mm/s2
#define MOT_2_ACCEL 500.0       // mm/s2
#define MOT_3_ACCEL 500.0       // mm/s2
#define MOT_HOMING_ACCEL 100.0       // mm/s2
#define MOT_HOMING_MAX_DISTANCE 3000 //mm

int16_t MOT_1_DIRECTION = -1;
int16_t MOT_2_DIRECTION = 1;
int16_t MOT_3_DIRECTION = 1;

int ledState = HIGH;       // the current state of the output pin
int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 10;   // the debounce time; increase if the output flickers

// variabili di sistema
uint16_t A; // variabile interna
uint16_t B; // variabile interna
uint16_t C; // variabile interna
uint16_t pA; // variabile interna
uint16_t pB; // variabile interna
uint16_t pC; // variabile interna
uint16_t A_MIN = 0;
uint16_t B_MIN = 0;
uint16_t C_MIN = 0;
uint16_t A_MAX = 1800; //altezza massima motore A
uint16_t B_MAX = 1800; //altezza massima motore B
uint16_t C_MAX = 1800; //altezza massima motore C
uint16_t UNDER_A_OFFSET = 0;   // gap aggiuntivo tra carrello inferiore e pavimento
uint16_t BETWEEN_B_OFFSET = 0; // gap aggiuntivo tra carrello centrale e i due esterni
uint16_t OVER_C_OFFSET = 0;    // gap aggiuntivo tra carrello superiore e soffitto
uint16_t CUSCINETTO = 0;       // come i tre sopra, misura unificata 
long initial_homing = 1;
int move_finished = 1;
boolean system_ready = false;
boolean motors_initialized = false;

namespace teensydmx = ::qindesign::teensydmx;
#ifdef MAINBOARD
teensydmx::Receiver dmxRx{Serial3}; // Create the DMX receiver on Serial1.
#endif
#ifdef PROTO
teensydmx::Receiver dmxRx{Serial1}; // Create the DMX receiver on Serial1.
#endif
FlexyStepper motor1;
FlexyStepper motor2;
FlexyStepper motor3;
