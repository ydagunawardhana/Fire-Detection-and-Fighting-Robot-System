#include "environmentMonitor.h"

// Create an instance for the DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

float h,t;
unsigned long lastCheckTime;

// Function to read MQ sensor
int readMQSensor() {
  // Read MQ sensor value
  return analogRead(MQ_PIN);
}

// Initialize DHT sensor
void initDHT() {
  dht.begin();
  delay(2000); // Wait for sensor to stabilize
}

// Read humidity
float readHumidity() {
  h = dht.readHumidity();  // Ensure `humidity` is accessed from thingProperties.h
  return h;
}

// Read temperature
float readTemperature() {
  t = dht.readTemperature();  // Ensure `temperature` is accessed from thingProperties.h
  return t;
}