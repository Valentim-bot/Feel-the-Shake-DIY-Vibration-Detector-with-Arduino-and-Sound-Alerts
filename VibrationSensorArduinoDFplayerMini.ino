#include <Wire.h> // Include the I2C communication library
#include <Adafruit_GFX.h> // Include the Adafruit GFX library for graphics functions
#include <Adafruit_SSD1306.h> // Include the Adafruit SSD1306 library for the OLED display
#include<SoftwareSerial.h>
#include<DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(10,11); //Rx, Tx
DFRobotDFPlayerMini myDFPlayer;


// OLED display configuration
#define SCREEN_WIDTH 128 // Define the OLED display width in pixels
#define SCREEN_HEIGHT 64 // Define the OLED display height in pixels
#define OLED_RESET -1 // Define the reset pin for the OLED display (-1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT); // Create an instance of the display object


// Vibration sensor pin
#define vibrationSensor 5 // Define the digital read pin for sensor input

// Timing variables for OLED display
unsigned long displayStartTime = 0; // Tracks when "Vibration Detected" is shown
bool showVibrationMessage = false;  // Flag to control message display
const long displayDuration = 2500;  // Duration to show message in milliseconds


// DFPlayer Mini timing variables
unsigned long previousMillis = 0; // Store the last time the DFPlayer was checked
const long interval = 2000; // Interval to wait for song playback in milliseconds
bool isPlaying = false; // Boolean to track if the DFPlayer is currently playing


void setup() {
  // put your setup code here, to run once:
  pinMode(vibrationSensor, INPUT);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  if(!myDFPlayer.begin(mySoftwareSerial)){

    Serial.println("Unable to begin: ");
    while(true);
  }
  myDFPlayer.volume(30); // Set volume value (0 up to 30)

  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check if the display initializes correctly
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Stay in an infinite loop if initialization fails
  }
  
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(F("Initializing..."));
  display.display();
  delay(2000);
  

}

void loop() {
   unsigned long currentMillis = millis(); // Get the current time in milliseconds

  // Read the vibration sensor state
  bool vibrationSensorState = digitalRead(vibrationSensor);

  // If vibration is detected
  if (vibrationSensorState == 1) {
    showVibrationMessage = true;         // Enable the message display
    displayStartTime = currentMillis;    // Store the current time
    isPlaying = true;                    // Set playback flag
    
    myDFPlayer.play(3);  // Play sound
  }

  // Check if 2.5 seconds have passed
  if (showVibrationMessage && (currentMillis - displayStartTime <= displayDuration)) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println(F("Vibration Detected"));
    display.display();
  } else {
    showVibrationMessage = false; // Reset flag
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(40, 20);
    display.println(F("NO"));
    display.setCursor(10, 43);
    display.println(F("Vibration"));
    display.display();
  }

  delay(100); // Short delay to avoid rapid updates
}
