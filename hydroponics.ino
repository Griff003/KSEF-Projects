#include "DHT.h"

// --- Pin Definitions ---
#define DHTPIN D8               // DHT sensor pin
#define DHTTYPE DHT11           // DHT 11 sensor type
#define MOISTURE_PIN D3         // Digital moisture sensor
#define PUMP_PIN D4             // Pump control
#define TURBIDITY_PIN A0        // Analog turbidity sensor

// --- Initialize DHT Sensor ---
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Smart Environment Monitor...");

  // Moisture & Pump setup
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  // Start DHT sensor
  dht.begin();
}

void loop() {
  // --- DHT Sensor ---
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  // --- Soil Moisture Sensor ---
  int moistureState = digitalRead(MOISTURE_PIN);
  if (moistureState == HIGH) {
    digitalWrite(PUMP_PIN, HIGH);  // Pump ON
    Serial.println("Soil is dry. Pump ON");
  } else {
    digitalWrite(PUMP_PIN, LOW);   // Pump OFF
    Serial.println("Soil is wet. Pump OFF");
  }

  // --- Turbidity Sensor ---
  int sensorValue = analogRead(TURBIDITY_PIN);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  turbidity = abs(turbidity);  // Ensure positive

  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Turbidity: ");
  Serial.print(turbidity);

  if (turbidity < 20) {
    Serial.println(" | Status: CLEAR");
  } else if (turbidity >= 20 && turbidity < 32) {
    Serial.println(" | Status: CLOUDY");
  } else {
    Serial.println(" | Status: DIRTY");
  }

  Serial.println("-----------------------------");
  delay(2000);  // 2-second delay for all systems
}
