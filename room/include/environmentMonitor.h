#ifndef ENVIRONMENT_MONITOR_H
#define ENVIRONMENT_MONITOR_H

#include <DHT.h>

// Pin and type definitions
#define DHT_PIN 15
#define DHT_TYPE DHT11
#define MQ_PIN 35

// Function prototypes
void initDHT();
float readHumidity();
float readTemperature();
int readMQSensor();

#endif // MQ_HUMIDITY_MONITOR_H