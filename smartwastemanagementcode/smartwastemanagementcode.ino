#include <Servo.h>
Servo servo;

// Pins
const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 7;
const int moisturePin = A0;

// Thresholds
const int moistureThreshold = 650;  // Between 580 (wet) and 600 (dry)
const int minDistance = 5;
const int maxDistance = 25;

long duration, distance;
int moistureValue;

int readMoisture() {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(moisturePin);
    delay(5);
  }
  return sum / 10;
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  servo.write(90); // Start neutral
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = (duration / 2.0) / 29.1;
}

void loop() {
  measureDistance();
  moistureValue = readMoisture();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Moisture: ");
  Serial.println(moistureValue);

  if (distance > minDistance && distance < maxDistance) {
    if (moistureValue < moistureThreshold) {  // Lower value = wet
      Serial.println("Wet waste - Opening Bin 1");
      servo.write(180);
    } else {
      Serial.println("Dry waste - Opening Bin 2");
      servo.write(0);
    }
    delay(2000);
    servo.write(90); // Return to center
  }

  delay(100);
}   