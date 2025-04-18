#include "arduino_secrets.h"
#include "environmentMonitor.h"
#include "fireDetection.h"
#include "communication.h"
#include "thingProperties.h"
#include <Arduino.h>

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  initDHT(); // Initialize DHT sensor
  pinMode(IR_FLAME_PIN_1, INPUT);
  pinMode(IR_FLAME_PIN_2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
    The following function allows you to obtain more information
    related to the state of network and IoT Cloud connection and errors.
    The higher the number the more granular the information.
    The default is 0 (only errors), maximum is 4.
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  initWifi();
}

void loop() {
  ArduinoCloud.update();

  // Read values from sensors
  float temp = readTemperature();
  float hum = readHumidity();
  int mq = readMQSensor();
  int state = detectFlame();

  // Update cloud variables
  temperature = temp;
  humidity = hum;
  mqValue = mq;
  room1 = state == 1;
  room2 = state == 2;

  // Debug prints
  Serial.print("🌡️ Temp: ");
  Serial.print(temperature);
  Serial.print("°C | 💧 Humidity: ");
  Serial.print(humidity);
  Serial.print("% | 🧪 MQ: ");
  Serial.print(mqValue);
  Serial.print(" | 🔥 Room 1: ");
  Serial.println(room1 ? "Yes" : "No");
  Serial.print("🔥 Room 2: ");
  Serial.println(room2 ? "Yes" : "No");

  fireControl(); // Check for fire detection

  delay(100); // Adjust as needed
}