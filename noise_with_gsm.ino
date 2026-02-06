#include <SoftwareSerial.h>

#define GSM_TX D2  // Connect GSM module TX to NodeMCU D1
#define GSM_RX D4  // Connect GSM module RX to NodeMCU D2

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

int raw_Value = A0;  // Analog pin connected to the sound sensor
int buzzer_pin = D1; // Pin connected to the buzzer

void setup() {
  Serial.begin(9600);   // Initialize serial communication for debugging
  gsmSerial.begin(9600); // Initialize GSM serial communication

  pinMode(raw_Value, INPUT);  // Set pin A0 as an input
  pinMode(buzzer_pin, OUTPUT); // Set buzzer pin as an output

  Serial.println("Initializing GSM module...");

  delay(2000); // Give time for the GSM module to initialize

  // AT command to test if the GSM module is responding
  gsmSerial.println("AT");
  delay(1000);
}

void loop() {
  int val_analog = analogRead(raw_Value);  // Read the analog value from the sound sensor

  Serial.print("Sound Level: ");
  Serial.println(val_analog);  // Print the analog value to the Serial Monitor

  // Check if the sound level exceeds 215
  if (val_analog > 360) {
    digitalWrite(buzzer_pin, HIGH);  // Turn on the buzzer
    Serial.println("Buzzer ON");     // Print a message to indicate buzzer is on

    // Send SMS when noise is detected
    sendSMS("The noise level is above the threshold");
    delay(5000); // Add a delay to avoid sending multiple SMS in quick succession
  } else {
    digitalWrite(buzzer_pin, LOW);   // Turn off the buzzer
  }

  delay(100);  // Add a small delay to avoid flooding the Serial Monitor
}

void sendSMS(const char* message) {
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);

  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  gsmSerial.print("AT+CMGS=\"0717492935\"\r");
  delay(1000);
  gsmSerial.print(message); // Send the message
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);

  Serial.println("SMS Sent!");
}
