#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include "GyroController.h"
#include "FireSuppression.h"

#define TARGET_ANGLE 70
#define OBSTACLE_THRESHOLD 19  // Set obstacle detection threshold in cm

#define MOTOR_A1 42  // IN1
#define MOTOR_A2 40  // IN2
#define ENABLE_A 2   // ENA (Enable pin)
#define MOTOR_B1 38  // IN3
#define MOTOR_B2 36  // IN4
#define ENABLE_B 3   // ENB (Enable pin)
#define TRIG_PIN 9   // Trigger pin for ultrasonic sensor
#define ECHO_PIN 10  // Echo pin for ultrasonic sensor
#define IR_RIGHT 28  // IR sensor pin


class MotorController {
public:
    MotorController(int motorA1, int motorA2, int enableA, int motorB1, int motorB2, int enableB, int trigPin, int echoPin,int irRight, GyroController &gyro, FireSuppressionSystem &fireSystem);
    void moveForward(int speed);
    void moveBackward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void controlCar();
    void brakeTurnLeft();
    void brakeTurnRight();
    void brake();
    void stopMoving();
    void stopMotors();
    int isRightDetected();
    float getDistance();
    bool isObstacleDetected();
    void moveToRoom(String roomNumber);

private:
    int motorA1, motorA2, enableA;
    int motorB1, motorB2, enableB;
    int trigPin, echoPin; // Ultrasonic sensor pins
    int irRight;
    GyroController *gyro;
    FireSuppressionSystem *fireSystem;
};

#endif