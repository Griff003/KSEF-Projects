#include <SoftwareSerial.h>

// Define GSM pins
#define GSM_TX D6 // Connect GSM TX to NodeMCU D6
#define GSM_RX D5 // Connect GSM RX to NodeMCU D5

// Define Flame Sensor analog pin
#define flamePin A0

// Define output pins
#define buzzerPin D8
#define ledPin D1

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);
bool messageSent = false; // To prevent spamming SMS

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.println("Initializing GSM module...");
  delay(2000);
  gsmSerial.println("AT");
  delay(1000);
}

void loop() {
  int flameValue = analogRead(flamePin);
  Serial.print("Flame Sensor Reading: ");
  Serial.println(flameValue);

  if (flameValue < 500) { // Adjust threshold if needed
    Serial.println("Flame Detected!");

    // Activate buzzer and LED
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);

    // Send SMS once
    if (!messageSent) {
      sendSMS("Flame detected! Take immediate action!");
      messageSent = true;
    }
  } else {
    Serial.println("No Flame.");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    messageSent = false; // Reset so next detection triggers SMS again
  }

  delay(1000);
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode
  delay(1000);
  gsmSerial.println("AT+CMGS=\"0740390420\""); // Replace with your number
  delay(1000);
  gsmSerial.print(message);
  delay(500);
  gsmSerial.write(26); // Ctrl+Z to send
  delay(5000);
  Serial.println("SMS sent.");
}
