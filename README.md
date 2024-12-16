# Robotics Pong Project

## Introduction


## Hardware design

### List of components

### Electrical schema
![Electrical_schema](https://github.com/user-attachments/assets/4b636e72-69dc-4217-b99d-9ff69293aaa8)

### Bloc schema
![Bloc_Schema](https://github.com/user-attachments/assets/37c18c2c-1ed0-43cd-8d78-f17d8894d4d6)

### Photo of setup
![WhatsApp Image 2024-12-16 at 19 10 27_d98df788](https://github.com/user-attachments/assets/e562884b-1d54-40d0-984a-96fa4da124e2)

To test the functionality of the LCD, I uploaded the following code to the Arduino board:
```cpp
#include <LiquidCrystal.h>
#include <Arduino.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  
}

void loop() {
  
}
 ```

## Software design


## Functionality

## Results and conclusions

## Journal


