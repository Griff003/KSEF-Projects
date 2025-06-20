#include <SoftwareSerial.h>

#define GSM_TX D1  // Connect GSM module TX to NodeMCU D1
#define GSM_RX D2  // Connect GSM module RX to NodeMCU D2

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

void setup() {
  Serial.begin(9600);   // Initialize serial communication for debugging
  gsmSerial.begin(9600); // Initialize GSM serial communication

  Serial.println("Initializing GSM module...");

  delay(2000); // Give time for the GSM module to initialize

  // AT command to test if the GSM module is responding
  gsmSerial.println("AT");
}

void loop() {
  if (gsmSerial.available()) {
    char c = gsmSerial.read();
    Serial.write(c); // Print response from GSM module to serial monitor
  }

  // Send SMS after 5 seconds (5000 milliseconds)
  delay(5000);
  sendSMS();
}

void sendSMS() {
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");

  delay(1000);

  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"0740390420\"\r");
  delay(1000);
  gsmSerial.print("Hello GSM");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}
