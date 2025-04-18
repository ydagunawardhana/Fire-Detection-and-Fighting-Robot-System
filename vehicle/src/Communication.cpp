#include "Communication.h"

Communication::Communication() {
    // Constructor
}

void Communication::initWifi() {
    Serial.println("Initializing ESP12E with AT Commands...");

    sendAT("AT", 2000); // Test
    sendAT("AT+CWMODE=1", 2000); // Station mode

    String connectCmd = "AT+CWJAP=\"" SECRET_SSID "\",\"" SECRET_PASS "\"";
    sendAT(connectCmd, 10000);  // Connect to Wi-Fi

    // Connect to ESP32 TCP Server at IP 192.168.8.200 port 80
    while (true) {
        bool connected = sendAT("AT+CIPSTART=\"TCP\",\"192.168.1.200\",80", 5000);
        if (connected) {
            digitalWrite(53, HIGH); // Turn off LED if not connected
            Serial.println("Connected to ESP32 TCP server.");
            break;
        } else {
            digitalWrite(53, LOW); // Turn on LED if not connected
            Serial.println("Failed to connect to ESP32 TCP server. Retrying...");
            delay(2000); // Wait before retrying
        }
    }

    Serial.println("ESP12E Wi-Fi Initialized (AT)");
}

String Communication::receiveRoomNumber() {
    if (Serial1.available()) {
        String response = Serial1.readStringUntil('\n');
        response.trim();
        Serial.print("Raw Room Number received: ");
        Serial.println(response);

        int colonIndex = response.indexOf(':');
        if (colonIndex != -1 && colonIndex + 1 < response.length()) {
            String roomNumber = response.substring(colonIndex + 1);
            roomNumber.trim();
            Serial.print("Cleaned Room Number: ");
            Serial.println(roomNumber);
            return roomNumber;
        }
    }
    return "";
}

// Helper function to send AT commands
bool Communication::sendAT(const String& cmd, unsigned long timeout = 2000) {
    Serial1.println(cmd);
    unsigned long start = millis();
    String response = "";

    while (millis() - start < timeout) {
        while (Serial1.available()) {
            char c = Serial1.read();
            response += c;
        }
        if (response.indexOf("OK") != -1 || response.indexOf("CONNECTED") != -1) {
            Serial.println("AT OK: " + cmd);
            return true;
        }
        if (response.indexOf("FAIL") != -1 || response.indexOf("ERROR") != -1) {
            Serial.println("AT ERROR: " + cmd);
            return false;
        }
    }

    Serial.println("AT TIMEOUT: " + cmd);
    return false;
}