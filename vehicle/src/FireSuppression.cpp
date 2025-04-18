#include "FireSuppression.h"
#include <Servo.h>

FireSuppressionSystem::FireSuppressionSystem() {
    this->pumpRelayPin = PUMP_RELAY_PIN;
    this->servoPin = SERVO_PIN;
    this->sensor1 = FLAME_SENSOR_1;
    this->sensor2 = FLAME_SENSOR_2;
    this->sensor3 = FLAME_SENSOR_3;
    this->isSuppressing = false;
}

void FireSuppressionSystem::initialize() {
    nozzleServo.attach(servoPin);
    pinMode(pumpRelayPin, OUTPUT);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
    digitalWrite(pumpRelayPin, HIGH); // Initialize relay as OFF
}

void FireSuppressionSystem::controlFireSystem() {
    state1 = digitalRead(sensor1);
    state2 = digitalRead(sensor2);
    state3 = digitalRead(sensor3);

    if((state1==LOW || state2==LOW || state3==LOW) && !isSuppressing) {
        activateSuppression();
    } else {
        stopSuppression();
    }
}

bool FireSuppressionSystem::isFireDetected() {
    state1 = digitalRead(sensor1);
    state2 = digitalRead(sensor2);
    state3 = digitalRead(sensor3);

    Serial.print("Sensor 1: ");
    Serial.println(state1 == LOW ? "Flame detected" : "No flame");
    Serial.print("Sensor 2: ");
    Serial.println(state2 == LOW ? "Flame detected" : "No flame");
    Serial.print("Sensor 3: ");
    Serial.println(state3 == LOW ? "Flame detected" : "No flame");

    bool fireDetected = (state1 == LOW || state2 == LOW || state3 == LOW);
    
    Serial.print("Fire Detected: ");
    Serial.println(fireDetected ? "YES" : "NO");
    
    return fireDetected;
}

void FireSuppressionSystem::activateSuppression() {
    if (!isSuppressing) {
        Serial.println("Activating Fire Suppression!");
        isSuppressing = true;
        digitalWrite(pumpRelayPin, LOW); // Turn on relay

        for (int i = 0; i < 180; i++) {
            nozzleServo.write(i);
            delay(25);
        }

        for (int i = 180; i > 0; i--) {
            nozzleServo.write(i);
            delay(25);
        }
        for (int i = 0; i < 180; i++) {
            nozzleServo.write(i);
            delay(25);
        }


        nozzleServo.write(90);
        stopSuppression();
        delay(1000);
        // gyroController.resetSystem();
        exit(0);
    }
}

void FireSuppressionSystem::stopSuppression() {
    digitalWrite(pumpRelayPin, HIGH); // Turn off relay
    isSuppressing = false;
}