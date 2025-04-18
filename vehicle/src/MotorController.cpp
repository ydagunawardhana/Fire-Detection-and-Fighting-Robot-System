#include "MotorController.h"
#include <Arduino.h>


MotorController::MotorController(int motorA1, int motorA2, int enableA, int motorB1, int motorB2, int enableB, int trigPin, int echoPin,int irRight, GyroController &gyro, FireSuppressionSystem &fireSystem) {
    this->motorA1 = motorA1;
    this->motorA2 = motorA2;
    this->enableA = enableA;
    this->motorB1 = motorB1;
    this->motorB2 = motorB2;
    this->enableB = enableB;
    this->trigPin = trigPin;
    this->echoPin = echoPin;
    this->irRight = irRight;
    this->gyro = &gyro;
    this->fireSystem = &fireSystem;

    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(enableA, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);
    pinMode(enableB, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(irRight, INPUT);
}
float MotorController::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Convert to cm
    return distance;
}

bool MotorController::isObstacleDetected() {
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);
    if (distance < OBSTACLE_THRESHOLD) {
        Serial.println("Obstacle detected!");
        return true;
    } else {
        Serial.println("No obstacle detected.");
        return false;
    }
}

void MotorController::moveForward(int baseSpeed) {
    Serial.println("Checking for obstacles and fire...");
    if (isObstacleDetected()) {
        Serial.println("Obstacle detected or fire detected! Stopping motors.");
        stopMoving();
    }
    Serial.println("Moving forward");
    
    float yawError = gyro->getYaw();
    float correction = yawError * 2.0; // Proportional control (tune this factor as needed)
    Serial.print("Yaw Error: ");
    Serial.println(yawError);

    int leftSpeed = constrain(baseSpeed + correction, 0, 255);
    int rightSpeed = constrain(baseSpeed - correction, 0, 255);
    
    analogWrite(enableA, leftSpeed);
    analogWrite(enableB, rightSpeed);
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
}

void MotorController::moveBackward(int speed) {
    analogWrite(enableA, speed);
    analogWrite(enableB, speed);
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
}

void MotorController::stopMoving() {
    Serial.println("Stopping motors");
    brake();
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    analogWrite(enableA, 0);
    analogWrite(enableB, 0);
    delay(2000);
}

void MotorController::stopMotors() {
    Serial.println("Stopping motors");
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    analogWrite(enableA, 0);
    analogWrite(enableB, 0);
}

void MotorController::turnLeft(int speed) {
    Serial.println("Turning left");
    float targetYaw = gyro->getYaw() - TARGET_ANGLE;
    stopMotors();

    analogWrite(enableA, speed);
    analogWrite(enableB, 0);
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);

    while (gyro->getYaw() > targetYaw) {
        Serial.println(gyro->getYaw());
        delay(10);
    }

    brakeTurnLeft();
    stopMotors();
    gyro->resetSystem();
}

void MotorController::turnRight(int speed) {
    Serial.println("Turning right");
    float targetYaw = gyro->getYaw() + TARGET_ANGLE;
    stopMotors();

    analogWrite(enableA, 0);
    analogWrite(enableB, speed);
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);

    while (gyro->getYaw() < targetYaw) {
        Serial.println(gyro->getYaw());
        delay(10);
    }

    brakeTurnRight();
    gyro->resetYaw();
}

void MotorController::controlCar() {
    int turningSpeed = 60;
    bool fireDetected = fireSystem->isFireDetected();

    // Obstacle detection logic
    if (isObstacleDetected()) {
        Serial.println("Turning right due to obstacle!");
        stopMoving();
        turnRight(turningSpeed);
    } 
    // Fire detection logic
    else if (fireDetected) {
        Serial.println("Fire detected! Stopping motors and activating suppression.");
        stopMoving();                    // Stop motors before suppressing fire
        fireSystem->activateSuppression(); // Activate fire suppression
    } 
    // Default case: move forward if no obstacle or fire detected
    else {
        Serial.println("No obstacle, no fire detected. Moving forward.");
        moveForward(60); 
    }
}
void MotorController::brake() {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(enableA, 150);
    analogWrite(enableB, 150);
    delay(50);
}

void MotorController::brakeTurnLeft() {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    analogWrite(enableA, 150);
    analogWrite(enableB, 0);
    
    delay(75);

    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
}

void MotorController::brakeTurnRight() {
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(enableA, 0);
    analogWrite(enableB, 150);

    delay(75);

    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
}

int MotorController::isRightDetected() {
    return(digitalRead(irRight)==LOW);
}

void MotorController::moveToRoom(String roomNumber) {
    int turningSpeed = 60;
    int speed = 60;

    if (roomNumber == "room1") {
        Serial.println("Navigating to Room 1"); 
        while (isRightDetected() && !isObstacleDetected() && !fireSystem->isFireDetected()) {
            moveForward(speed);
            delay(20); // gives time for yaw to be updated
        }

        if (!isRightDetected() && !isObstacleDetected()) {
            Serial.println("Room 1 detected, turning right");
            delay(100);
            stopMoving();
            delay(2000); // Wait for 2 seconds
            turnRight(turningSpeed);
            Serial.println("Successfully turned right");
            delay(2000); // Adjust delay as needed
            while (!fireSystem->isFireDetected() && !isObstacleDetected()) {
                moveForward(speed);
                delay(100); // Small delay to avoid overwhelming the system
            }
            stopMoving();
            if (fireSystem->isFireDetected()) {
                Serial.println("Fire detected while moving to Room 1. Activating suppression.");
                fireSystem->activateSuppression();
            } else if (isObstacleDetected()) {
                Serial.println("Obstacle detected while moving to Room 1. Stopping.");
            }
        } else if (isObstacleDetected()) {
            Serial.println("Obstacle detected, stopping");
            stopMotors();
        }
    } else if(roomNumber == "room2"){
        Serial.println("Navigating to Room 2"); 
        while (!isObstacleDetected()) {
            Serial.print("Checking for obstacles...");
            Serial.println(isObstacleDetected());
            moveForward(speed);
            delay(50); // gives time for yaw to be updated
        }

        if (!isRightDetected() && isObstacleDetected()) {
            Serial.println("Room 2 detected, turning right");
            delay(100);
            stopMoving();
            delay(2000); // Wait for 2 seconds
            turnRight(turningSpeed);
            Serial.println("Successfully turned right");
            delay(2000); // Adjust delay as needed
            while (!fireSystem->isFireDetected() && !isObstacleDetected()) {
                moveForward(speed);
                delay(100); // Small delay to avoid overwhelming the system
            }
            stopMoving();
            if (fireSystem->isFireDetected()) {
                Serial.println("Fire detected while moving to Room 1. Activating suppression.");
                fireSystem->activateSuppression();
            } else if (isObstacleDetected()) {
                Serial.println("Obstacle detected while moving to Room 1. Stopping.");
            }
        }
    } 
}