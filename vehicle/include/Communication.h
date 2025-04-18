#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "arduino_secrets.h"

class Communication {
public:
    Communication();
    void initWifi();
    String receiveRoomNumber();

private:
    bool sendAT(const String& cmd, unsigned long timeout);
    int LED_PIN = 53; // Pin for LED
};

#endif