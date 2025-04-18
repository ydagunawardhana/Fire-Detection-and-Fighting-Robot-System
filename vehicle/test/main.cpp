#include <Arduino.h>

const char *ssid = "Dialog 4G 875";       // Your Wi-Fi SSID
const char *password = "f45902DF"; // Your Wi-Fi password

void sendHttpRequest();

void setup() {
  // Start serial communication with Arduino Mega
  Serial.begin(115200);

  // Start serial communication with ESP12E on Serial1 (hardware serial)
  Serial1.begin(115200);  // Default baud rate of ESP12E

  // Wait for ESP12E to initialize
  delay(2000);

  // Test if ESP12E is responding to AT commands
  Serial.println("Testing ESP12E response to AT command...");
  Serial1.println("AT");
  delay(1000);

  // Send AT commands to ESP12E to connect to Wi-Fi
  Serial.println("Sending AT+RST to reset ESP12E...");
  Serial1.println("AT+RST");  // Reset ESP12E
  delay(1000);

  Serial.println("Sending AT+CWMODE=1 to set ESP12E to Wi-Fi station mode...");
  Serial1.println("AT+CWMODE=1");  // Set ESP12E to Wi-Fi station mode
  delay(1000);

  Serial.println("Sending AT+CWJAP to connect to Wi-Fi...");
  Serial1.print("AT+CWJAP=\"");
  Serial1.print(ssid);
  Serial1.print("\",\"");
  Serial1.print(password);
  Serial1.println("\"");  // Connect to Wi-Fi

  delay(10000);  // Wait for Wi-Fi connection

  // Check if connected to Wi-Fi
  Serial.println("Sending AT+CIFSR to get IP address...");
  Serial1.println("AT+CIFSR");  // Get IP address
  delay(2000);

  // Print ESP12E's response to Serial Monitor
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // Connect to ESP32 server and send HTTP request
  sendHttpRequest();
}

void loop() {
  // If ESP12E sends data, print it to the serial monitor
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // If Arduino sends data, pass it to ESP12E
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}

// Function to send an HTTP request to ESP32
void sendHttpRequest() {
  // Send HTTP request to ESP32 server
  String request = "AT+CIPSTART=\"TCP\",\"192.168.8.139\",80";  // Connect to ESP32 IP
  Serial1.println(request);
  delay(2000);  // Wait for connection
  
  String requestLength = "AT+CIPSEND=" + String(request.length());
  Serial1.println(requestLength);  // Send HTTP request length
  delay(1000);

  Serial1.println("GET / HTTP/1.1\r\nHost: 192.168.8.139\r\nConnection: keep-alive\r\n\r\n"); // HTTP GET request
  delay(1000);  // Wait for the request to be sent
}