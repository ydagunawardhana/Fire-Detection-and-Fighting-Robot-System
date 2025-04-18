#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "arduino_secrets.h"

// Function prototype
void initWifi();
void sendRoomNumber(String roomNumber);

#endif