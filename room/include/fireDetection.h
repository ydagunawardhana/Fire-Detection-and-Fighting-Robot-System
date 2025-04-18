#ifndef FIRE_DETECTION_H
#define FIRE_DETECTION_H

#include <Arduino.h>

// Define sensor and buzzer pins
#define IR_FLAME_PIN_1 4
#define IR_FLAME_PIN_2 5
#define BUZZER_PIN 25

// Function prototypes
int detectFlame();
void fireControl();
void alertFire();
void alertBuzzer(int frequency);
void stopBuzzer();

#endif