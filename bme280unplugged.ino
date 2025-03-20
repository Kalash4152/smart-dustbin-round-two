#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

// Create instances for BME280 and LCD
Adafruit_BME280 bme; // I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your LCD's I2C address if needed

// Define pins for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Define distance threshold
const int distanceThreshold = 4; // in cm

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize the BME280 sensor
  if (!bme.begin(0x76)) { // Change to 0x76 if needed
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Initialize the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // Read distance from ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Read BME280 sensor values
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa

  // Display readings on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  // Check if waste is full
  if (distance <= distanceThreshold) {
    lcd.setCursor(0, 1);
    lcd.print("Waste is Full!");
    delay(2000); // Display message for 2 seconds
  }

  delay(1000); // Update every second
}