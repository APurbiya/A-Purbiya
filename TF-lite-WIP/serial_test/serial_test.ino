#include <Adafruit_NeoPixel.h>

#define LED_PIN     9
#define NUM_LEDS    12

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

char receivedData[50];
char direction;
int boxSize;
unsigned long previousMillis = 0;
const long interval = 500; // Flash interval in milliseconds

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming data
    Serial.readBytesUntil('\n', receivedData, sizeof(receivedData));

    // Parse the received data
    direction = receivedData[0];
    boxSize = atoi(&receivedData[2]); // Assuming the box size is sent as a number after a comma

    // Print received data for debugging
    Serial.print("Direction: ");
    Serial.println(direction);
    Serial.print("Box Size: ");
    Serial.println(boxSize);

    // Clear the LED strip
    strip.clear();

    // Implement conditional logic based on the received data
    if (direction == 'L') {
      // Turn on lights 6-11
      for (int i = 6; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, getColor(boxSize));
      }
    } 
    else if (direction == 'R') {
      // Turn on lights 0-5
      for (int i = 0; i < 6; i++) {
        strip.setPixelColor(i, getColor(boxSize));
      }
    }
    else if (direction == 'X')
    {
      for (int i = 0; i < 11; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
      }
    }

    // Show the updated LED colors
    strip.show();

    // Reset received data buffer
    memset(receivedData, 0, sizeof(receivedData));
  }

  // Flashing red for really big box sizes
  if (direction != '\0' && boxSize > 1000) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // Toggle the LEDs on even indices
      for (int i = 0; i < NUM_LEDS; i += 2) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
      }
      strip.show();
      // Save the current time for the next interval
      previousMillis = currentMillis;
    } else {
      // Turn off all LEDs
      strip.clear();
      strip.show();
    }
  }
}

// Function to determine LED color based on box size
uint32_t getColor(int boxSize) {
  if (boxSize > 2000) {
    Serial.println("Green");
    return strip.Color(0, 255, 0); // Green
  } 
  else if (boxSize < 7000 && boxSize > 2001) {
    Serial.println("Yellow");
    return strip.Color(255, 255, 0); // Yellow
  } 
  else {
    Serial.println("Red");
    return strip.Color(255, 0, 0); // Red
  }
}
