#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
#define I2C_ADDR    0x27   // Set the correct I2C address of your LCD
#define LCD_COLUMNS 16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

// Keypad setup
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {4, 5, 6, 7}; // Row pins connected to the ESP32
byte colPins[COLS] = {11, 16, 17, 18}; // Column pins connected to the ESP32

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int secretNumber;
char guess;

void setup() {
  Serial.begin(115200);
  
    // Initialize LCD
  Wire.begin(36, 35);  // SDA on GPIO 36, SCL on GPIO 35
  lcd.init();
  lcd.backlight();
  
  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("Guess the number!");
  lcd.setCursor(0, 1);
  lcd.print("1-9: ");
  delay(2000);

  // Seed the random number generator
  randomSeed(analogRead(0));
  
  // Generate random number between 1 and 9
  secretNumber = random(1, 10);
  Serial.println("Welcome to the Number Guessing Game!");
  Serial.println("Guess the number (1-9):");
}


void loop() {
  char key = keypad.getKey();  // Get the key press

  if (key) {
    // Debugging: Print the actual key received
    Serial.print("You guessed: ");
    Serial.println(key);  // Check the exact character received


    // Only proceed if the key is a valid number
    if (key >= '1' && key <= '9') {
      guess = key - '0';  // Convert char to integer

      if (guess == secretNumber) {
        Serial.println("Correct! You guessed the right number!");
        // Display success message on LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct! Winner!");
        delay(2000);
        
        // Reset the game
        secretNumber = random(1, 10);
        Serial.println("New number generated, guess again:");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("New game. Guess!");

      } else {
        if (guess > secretNumber) {
          Serial.println("Lower! Try again.");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("You guessed: ");
          lcd.print(key);
          lcd.setCursor(0, 1);
          lcd.print("Lower! Try again.");
        } else {
          Serial.println("Higher!Try again.");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("You guessed: ");
          lcd.print(key);
          lcd.setCursor(0, 1);
          lcd.print("Higher!Try again.");
        }
      }
    } else {
      Serial.println("Please enter a number between 1 and 9.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please enter a number");
      lcd.setCursor(0, 0);
      Serial.println("between 1 and 9.");
    }
  }
}