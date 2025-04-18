#include "fireDetection.h"
#include "communication.h"

void fireControl() {
    int flameDetected = detectFlame();  // Store the result once

    if (flameDetected == 1) {
        Serial.println("🔥 Fire detected in Room 1!");
        sendRoomNumber("room1");  // Send room number to HC-06
        alertFire();
    } else if (flameDetected == 2) {
        Serial.println("🔥 Fire detected in Room 2!");
        sendRoomNumber("room2");  // Send room number to HC-06
        alertFire();
    } else {
        Serial.println("✅ No fire detected.");
        stopBuzzer();
    }
}

int detectFlame() {
    int room1State = digitalRead(IR_FLAME_PIN_1);
    int room2State = digitalRead(IR_FLAME_PIN_2) == LOW;

    Serial.print("📡 Room 1 Sensor: ");
    Serial.println(room1State);
    Serial.print("📡 Room 2 Sensor: ");
    Serial.println(room2State);

    if (room1State == HIGH) return 1;
    if (room2State == HIGH) return 2;
    
    return 0;
}

void alertFire() {
    for (int i = 0; i < 3; i++) {
        alertBuzzer(1000);
        delay(500);
        stopBuzzer();
        delay(500);
    }
}

void alertBuzzer(int frequency) {
    tone(BUZZER_PIN, frequency);
}

void stopBuzzer() {
    noTone(BUZZER_PIN);
}