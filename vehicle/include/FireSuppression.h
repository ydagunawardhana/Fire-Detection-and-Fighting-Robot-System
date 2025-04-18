#ifndef FIRE_SUPPRESSION_H
#define FIRE_SUPPRESSION_H

#include "GyroController.h"
#include <Arduino.h>
#include <Servo.h>

// Fire Suppression System Pins
#define PUMP_RELAY_PIN 30
#define SERVO_PIN 8
#define FLAME_SENSOR_1 24
#define FLAME_SENSOR_2 22
#define FLAME_SENSOR_3 26

class FireSuppressionSystem {
public:
    FireSuppressionSystem();
    void initialize();
    void controlFireSystem();
    void activateSuppression();
    void stopSuppression();
    bool isFireDetected();
    GyroController gyroController;

private:
    int pumpRelayPin;
    int servoPin;
    int sensor1, sensor2, sensor3;
    int state1, state2, state3;
    Servo nozzleServo;
    bool isSuppressing;
};

#endif // FIRE_SUPPRESSION_SYSTEM_H