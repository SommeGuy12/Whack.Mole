#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD address and size

// LED and Button pin definitions
const int ledPins[] = {2, 3, 4, 5, 6};     // Red, Blue, Green, Yellow, Orange LEDs
const int buttonPins[] = {11, 10, 9, 8, 7}; // Buttons for the LEDs

const int buzzerPin = 13;

long timeLimit = 60000;  // Time limit in milliseconds
long startTime = 0;
int score = 0;

// Timer variables to update every second
unsigned long lastUpdateTime = 0; // Last time we updated the timer
const long updateInterval = 1000;  // 1 second

// Game state variables
bool ledOn = false;      // Tracks if an LED is currently on
int currentLed = -1;     // Stores the index of the currently lit LED

void setup() {
  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on backlight
  
  // Initialize LED pins as outputs
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  pinMode(buzzerPin, OUTPUT);  // Initialize buzzer
  
  // Initialize button pins as inputs
  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  
  startTime = millis();  // Start the game timer
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Update the timer every second
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis; // Store the current time
    
    // Calculate time left
    int timeLeft = (timeLimit - (currentMillis - startTime)) / 1000;
    lcd.setCursor(0, 0);
    lcd.print("Time Left: ");
    lcd.print(timeLeft);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
  }

  if (currentMillis - startTime < timeLimit) {
    game();  // Run the game logic
  } else {
    // Time is up, show the final score
    lcd.setCursor(0, 0);
    lcd.print("Time's up!              ");
    GameOver();
  }
}

void game() {
  if (!ledOn) {  // If no LED is on, randomly turn one on
    currentLed = random(0, 5); // Choose a random LED index
    digitalWrite(ledPins[currentLed], HIGH);  // Turn on the selected LED
    ledOn = true;  // Set flag to indicate that an LED is on
  }
  
  // Check if the button for the LED is pressed
  if (digitalRead(buttonPins[currentLed]) == HIGH) {
    // Correct button press, turn off the LED
    digitalWrite(ledPins[currentLed], LOW);
    score++;  // Increment score
    ledOn = false;  // Reset flag to indicate that the LED has been turned off
    tone(buzzerPin, 1000);
    delay(300);  // Small delay to prevent multiple presses
    noTone(buzzerPin);
  }
}

void GameOver() {
  // Turn on all LEDs and play buzzer sound
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  tone(buzzerPin, 1000);  // Play buzzer sound
  
  // Display final score on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.print(score);
  
  delay(1000);  // Wait for 1 second

  // Turn off all LEDs and buzzer
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  noTone(buzzerPin);

  delay(2000);  // Wait for 2 seconds

  // Show reset message
  lcd.setCursor(0, 0);
  lcd.print("Please reset game");
  delay(5000);  // Wait for 5 seconds before the next round
}