#include "GyroController.h"

GyroController::GyroController() {
    // Constructor
}

void GyroController::initialize() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");
    calibrateGyro();
    lastTime = millis();
}

float GyroController::getYaw() {
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0; // Time difference in seconds
    lastTime = currentTime;

    mpu.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    // Gyroscope Z-axis value in degrees per second
    float gyroZ = gyroEvent.gyro.z * 57.2958 - gyroZ_offset; // Convert rad/s to deg/s

    // Integrate gyroscope data to get yaw angle
    currentYaw += gyroZ * dt;

    return currentYaw;
}

void GyroController::calibrateGyro() {
    Serial.println("Calibrating gyroscope...");
    float sum = 0;
    int n = 500;

    for (int i = 0; i < n; i++) {
        mpu.getEvent(&accelEvent, &gyroEvent, &tempEvent);
        sum += gyroEvent.gyro.z * 57.2958; // Convert rad/s to deg/s
        delay(5);
    }

    gyroZ_offset = sum / n;
    Serial.print("Gyro Z Offset: ");
    Serial.println(gyroZ_offset);
    Serial.println("Calibration complete!");
}

void GyroController:: resetYaw() {
    Serial.println("Yaw reset");
    currentYaw = 0;
    lastTime = millis();
}

void GyroController::resetSystem() {
    Serial.println("System Resetting...");

    // Enable Watchdog Timer for 15ms timeout
    wdt_enable(WDTO_15MS);

    // Wait for the watchdog to trigger a reset
    while (true) {}
}