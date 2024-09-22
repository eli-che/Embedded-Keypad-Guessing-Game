# Embedded Keypad Guessing Game

This project implements a simple number guessing game using an ESP32, a 4x4 matrix keypad, and a 16x2 I2C LCD. Players input their guesses through the keypad, and the system provides feedback on whether the guessed number is higher or lower than the secret number.

## Features
https://github.com/user-attachments/assets/8b2819c1-da95-4d95-84f4-7ead171bb430

- **Number Guessing Game**: A randomly generated number between 1 and 9 must be guessed by the player.
- **Keypad Input**: The player inputs their guesses using a 4x4 matrix keypad.
- **LCD Feedback**: The game displays hints (Higher/Lower) and messages on the 16x2 I2C LCD.
- **Serial Monitoring**: The game outputs all interactions to the serial monitor for debugging.

## Components
- **ESP32 (ESP32-S3-WROOM)**
- **4x4 Matrix Keypad**
- **16x2 I2C LCD**
- **Wires & Breadboard**

## Pin Configuration

| Component           | GPIO Pin      | Function          |
|---------------------|---------------|-------------------|
| Keypad Rows         | GPIO 4, 5, 6, 7 | Keypad input row pins |
| Keypad Columns      | GPIO 11, 16, 17, 18 | Keypad input column pins |
| LCD SDA             | GPIO 36       | I2C Data Line     |
| LCD SCL             | GPIO 35       | I2C Clock Line    |

## Circuit Diagram
- Connect the keypad rows to GPIO pins 4, 5, 6, and 7.
- Connect the keypad columns to GPIO pins 11, 16, 17, and 18.
- Connect the I2C LCD's SDA to GPIO 36 and SCL to GPIO 35.

## Code Explanation
The project uses the **Keypad** and **LiquidCrystal_I2C** libraries to interact with the keypad and display messages on the LCD. A random number between 1 and 9 is generated, and the player must guess the number using the keypad. Feedback is provided on the LCD, and the game resets once the player guesses correctly.

```cpp
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
#define I2C_ADDR    0x27   
#define LCD_COLUMNS 16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {4, 5, 6, 7};
byte colPins[COLS] = {11, 16, 17, 18};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int secretNumber;
char guess;

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  Wire.begin(36, 35);  // SDA on GPIO 36, SCL on GPIO 35
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Guess the number!");
  delay(2000);
  
  randomSeed(analogRead(0));
  secretNumber = random(1, 10);
  Serial.println("Guess the number (1-9):");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("You guessed: ");
    Serial.println(key);

    if (key >= '1' && key <= '9') {
      guess = key - '0';

      if (guess == secretNumber) {
        Serial.println("Correct! You guessed the right number!");
        lcd.clear();
        lcd.print("Correct! Winner!");
        delay(2000);
        secretNumber = random(1, 10);
        lcd.clear();
        lcd.print("New game. Guess!");
      } else {
        if (guess > secretNumber) {
          lcd.clear();
          lcd.print("You guessed: ");
          lcd.print(key);
          lcd.setCursor(0, 1);
          lcd.print("Lower! Try again.");
        } else {
          lcd.clear();
          lcd.print("You guessed: ");
          lcd.print(key);
          lcd.setCursor(0, 1);
          lcd.print("Higher! Try again.");
        }
      }
    } else {
      lcd.clear();
      lcd.print("Please enter 1-9");
    }
  }
}
```

## How to Use
1. **Hardware Setup**:
   - Connect the keypad and LCD according to the pin configuration table.
2. **Software Setup**:
   - Install the required libraries: [Keypad Library](https://playground.arduino.cc/Code/Keypad), [LiquidCrystal_I2C Library](https://github.com/johnrickman/LiquidCrystal_I2C).
   - Upload the provided code to your ESP32 using the Arduino IDE.
3. **Run the Project**:
   - Open the serial monitor for debugging.
   - Use the keypad to guess the number (1-9), and follow the feedback on the LCD.
   - The game resets after a correct guess.

## Future Improvements
- Expand the range of the secret number to include more digits.
- Add difficulty levels with increasing ranges or fewer attempts.
- Implement a scoring system based on the number of tries taken to guess correctly.

## License
This project is open-source under the MIT License. Feel free to modify and distribute as needed.
