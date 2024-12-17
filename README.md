# Robotics Pong Project

## Introduction


## Hardware design

### List of components
- LCD
  [https://docs.arduino.cc/learn/electronics/lcd-displays/]
- 8*8 LED Matrix with shield {https://ardushop.ro/ro/home/95-matrice-led-uri-8x8-circuit-de-control.html?gad_source=1&gbraid=0AAAAADlKU-7WjbQn1szImTGPoCJSqEoDc&gclid=CjwKCAiA9bq6BhAKEiwAH6bqoHuTmyFDjMcYbo7vP07UJx7_Xq1HnsRM9HsLeTqa0WV30v7ykXhSoxoCUNsQAvD_BwE]
- Buzzer
- 2 Joysticks [https://components101.com/modules/joystick-module]
- 2 Potentiometers (for buzzer volume and LCD brightness control)
- Arduino Uno R3 board
- Large breadbord
- Jumper wire
- Resistors (330 ohms)

### Electrical schema
![Electrical_schema](https://github.com/user-attachments/assets/4b636e72-69dc-4217-b99d-9ff69293aaa8)

### Bloc schema
![Bloc_Schema](https://github.com/user-attachments/assets/37c18c2c-1ed0-43cd-8d78-f17d8894d4d6)

### Pin configuration

1. LCD:
- pin k -> gnd
- pin A -> paired with a 220 ohm resistor (datasheet) 
- pin D7 -> 2
- pin D6 -> 3
- pin D5 -> 4
- pin D4 -> 5
- pin VSS -> gnd
- pin VDD -> 5V
- pin RS (LCD) -> 7 Arduino
- pin RW (LCD) -> gnd
- pin E (LCD) -> 6 Arduino
- pin Vo (LCD) -> middle pin potentiometer
- left pin potentiometer -> gnd | righ pin potentiometer -> 5V

2. Joysticks:  (Joystick 1 -> brown gnd cable | Joystick 2 -> purple gnd cable)
- pin gnd -> gnd (for both)
- pin 5v -> + (for both)
- Joystick 1:
   - pin vrx -> A0 arduino
   - pin vry -> A1 Arduino
   - pin SW -> 8 Arduino (digital)


### Photo of setup
![WhatsApp Image 2024-12-16 at 19 10 27_d98df788](https://github.com/user-attachments/assets/e562884b-1d54-40d0-984a-96fa4da124e2)

To test the functionality of the LCD, I uploaded the following code to the Arduino board:
```cpp
#include <LiquidCrystal.h>
#include <Arduino.h>

// initializing the library by associating the lcd pins with the respective arduino pins that it s connected to
const int rs = 7, en = 6,  d7 = 2, d6 = 3, d5 = 4, d4 = 5, 
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


