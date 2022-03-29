#define SVILUPPO // PROTOTYPE per pcb Delio || SVILUPPO per default

#ifdef PROTOTYPE
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

#define UP_ENDSTOP_PIN 4
#define MID_UP_ENDSTOP_PIN 3
#define MID_DOWN_ENDSTOP_PIN 2
#define DOWN_ENDSTOP_PIN 1
#endif
#ifdef SVILUPPO
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

#define UP_ENDSTOP_PIN 11
#define MID_UP_ENDSTOP_PIN 12
#define MID_DOWN_ENDSTOP_PIN 13
#define DOWN_ENDSTOP_PIN 14
#endif

// configurazione DMX
#define dmxStartChannel 1

// configurazione stepper
#define MOT_1_STEPS_PER_MM 50.0 // step/mm
#define MOT_2_STEPS_PER_MM 50.0 // step/mm
#define MOT_3_STEPS_PER_MM 50.0 // step/mm
#define MOT_1_SPEED 5000.0      // mm/s
#define MOT_2_SPEED 5000.0      // mm/s
#define MOT_3_SPEED 5000.0      // mm/s
#define MOT_1_ACCEL 500.0       // mm/s2
#define MOT_2_ACCEL 500.0       // mm/s2
#define MOT_3_ACCEL 500.0       // mm/s2

// sistema
uint16_t A; // variabile interna
uint16_t B; // variabile interna
uint16_t C; // variabile interna
uint16_t pA; // variabile interna
uint16_t pB; // variabile interna
uint16_t pC; // variabile interna
uint16_t A_MIN = 0;
uint16_t B_MIN = 0;
uint16_t C_MIN = 0;
uint16_t A_MAX = 100;
uint16_t B_MAX = 100;
uint16_t C_MAX = 100;
uint16_t UNDER_A_OFFSET = 0;   // Gap aggiuntivo tra carrello inferiore e pavimento
uint16_t BETWEEN_B_OFFSET = 0; // gap aggiuntivo tra carrello centrale e i due esterni
uint16_t OVER_C_OFFSET = 0;    // gap aggiuntivo tra carrello superiore e soffitto