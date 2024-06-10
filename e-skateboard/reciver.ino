#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial hc06(0, 1); // RX, TX
Servo myServo; // Create servo object to control a servo

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");

  // Initialize Bluetooth Serial Port
  hc06.begin(9600);

  // Set pin 3 as GND and pin 4 as VCC
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3, LOW); // Set pin 3 to GND
  digitalWrite(4, HIGH); // Set pin 4 to VCC

  // Attach the servo to pin 5
  myServo.attach(5);

  // Set initial servo position
  myServo.write(0); // start at 0 degrees
}

void loop() {
  // Write data from HC06 to Serial Monitor
  if (hc06.available()) {
    char receivedChar = hc06.read();
    Serial.println(receivedChar);
    Serial.write(receivedChar);

    // Check if the received character is between '1' and '9'
    if (receivedChar >= '1' && receivedChar <= '9') {
      int number = receivedChar - '0'; // Convert char to int
      int angle = map(number, 1, 9, 30, 200); // Map number to angle between 0 and 180
      myServo.write(angle); // Set servo to the mapped angle
    }
    if(receivedChar == '0')
    {
      myServo.write(0); // start at 0 degrees
    }
  }

  // Write from Serial Monitor to HC06
  if (Serial.available()) {
    hc06.write(Serial.read());
  }
}
