#include <Servo.h>

// Define pins
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 11;

// Create a Servo object
Servo myServo;

// Define distance threshold
const int distanceThreshold = 5; // in cm
const int openDuration = 10000;   // in milliseconds

void setup() {
  // Initialize the servo
  myServo.attach(servoPin);
  myServo.write(0); // Start with the lid closed

  // Initialize the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600); // For debugging
}

void loop() {
  long duration, distance;

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin, return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  distance = duration * 0.034 / 2;

  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if the distance is less than or equal to the threshold
  if (distance <= distanceThreshold) {
    openLid();
  }

  delay(100); // Small delay to avoid rapid triggering
}

void openLid() {
  myServo.write(90); // Open the lid (adjust angle as needed)
  delay(openDuration); // Keep the lid open for the specified duration
  myServo.write(0); // Close the lid
}