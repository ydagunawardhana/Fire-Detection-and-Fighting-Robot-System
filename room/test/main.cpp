#include <WiFi.h>

const char *ssid = "Dialog 4G 875";       // Your Wi-Fi SSID
const char *password = "f45902DF"; // Your Wi-Fi password

WiFiServer server(80); // Start the server on port 80

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Wait for a client to connect
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");

    // Send a welcome message once the client connects
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: keep-alive");
    client.println();  // Blank line before the message
    client.println("Hello from ESP32 server. Waiting for further messages...");

    // Continuously send messages to the client (Arduino Mega)
    while (client.connected()) {
      String message = "Continuous message from ESP32: " + String(millis());
      client.println(message);  // Send message with current timestamp
      delay(10);  // Wait 1 second between messages
    }

    client.stop();  // Stop the client connection once it's no longer connected
    Serial.println("Client disconnected");
  }
}