#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>

// Define pins for ultrasonic sensor
const int trigPin = 23;
const int echoPin = 22;

// Create instances for web server and Bluetooth
WebServer server(80);
BluetoothSerial SerialBT;

// Define distance threshold
const int distanceThreshold = 4; // in cm

// Wi-Fi credentials
const char* ssid = "Your_SSID"; // Replace with your Wi-Fi SSID
const char* password = "Your_PASSWORD"; // Replace with your Wi-Fi password

void setup() {
  Serial.begin(115200);
  
  // Initialize the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start Bluetooth
  SerialBT.begin("ESP32_Dustbin"); // Bluetooth device name

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the web server
  server.on("/", handleRoot);
  server.on("/check", handleCheck);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Handle incoming client requests

  // Check distance from ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // If the distance is less than or equal to the threshold, send notification
  if (distance <= distanceThreshold) {
    SerialBT.println("Dustbin is Full!");
    delay(5000); // Avoid sending multiple notifications
  }
}

void handleRoot() {
  String html = "<html><body><h1>Dustbin Status</h1>";
  html += "<p>Check the dustbin status via Bluetooth notifications.</p>";
  html += "<p><a href=\"/check\">Check Dustbin Status</a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCheck() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  String message;
  if (distance <= distanceThreshold) {
    message = "Dustbin is Full!";
  } else {
    message = "Dustbin is Not Full.";
  }
  
  server.send(200, "text/plain", message);
}