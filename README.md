# Robotics Pong Project

## Introduction
For my robotics project I have decided to make a personalized version of the well-known PONG game. 
It is one of the most popular games of all time, and it offers a lot of versatility for both the hardware and the software implementation.

## Hardware design

### List of components (BOM)
- LCD
  [https://docs.arduino.cc/learn/electronics/lcd-displays/]
- 8*8 LED Matrix with shield {https://ardushop.ro/ro/home/95-matrice-led-uri-8x8-circuit-de-control.html?gad_source=1&gbraid=0AAAAADlKU-7WjbQn1szImTGPoCJSqEoDc&gclid=CjwKCAiA9bq6BhAKEiwAH6bqoHuTmyFDjMcYbo7vP07UJx7_Xq1HnsRM9HsLeTqa0WV30v7ykXhSoxoCUNsQAvD_BwE] <br>
Video that helped: [https://www.youtube.com/watch?v=sHNohdM_HJ4&t=136s]
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

### Hardware functionality
The **LCD** is initially responsible with displaying the greeting message (when the board is powered up) and then with the main menu.
The main menu has 4 sections :
- Start game
- Difficulty 
- How to play instructions
- About section
Once the game begins, it is tasked with displaying a countdown, and various messages during the game (such as when scoring/winning a match).

The **Joysticks** are tasked with moving the pong pallets during gameplay. 
One of them will also control all options in the LCD menu.

The **LED matrix** is tasked with displaying the actual game, the ball and 2 pallets. Pallets will be displayed as 3 dots at the edge of the matrix,
the bal will be displayed as 1 dot, appearing in the center at the beginning. Connected through SPI (Serial Peripheral Interface)

The 2 **potentiometers** are individually tasked with controlling the LCD brightness and the buzzer tone(frequency).

The **buzzer** is responsible for all the sounds in the game. Small short sounds for scrolling in the menu, or hitting the ball during gameplay.


### Pin configuration

1. LCD:
- pin k -> gnd
- pin A -> paired with a 330 ohm resistor (datasheet) 
- pin D7 -> 2 Arduino (digital)
- pin D6 -> 3 Arduino (digital)
- pin D5 -> 4 Arduino (digital)
- pin D4 -> 5 Arduino (digital)
- pin VSS -> gnd
- pin VDD -> 5V
- pin RS (LCD) -> 7 Arduino (digital) (for register selection)
- pin RW (LCD) -> gnd (Connected to ground for write mode)
- pin E (LCD) -> 6 Arduino (digital)
- pin Vo (LCD) -> middle pin potentiometer (contrast adjustment pin)
- left pin potentiometer -> gnd | righ pin potentiometer -> 5V
We are using pins D4-D7 for 4 bit mode. (D0-D3 are for 8 bit mode).

2. Joysticks:  (Joystick 1 -> brown gnd cable | Joystick 2 -> purple gnd cable)
- pin gnd -> gnd (for both)
- pin 5v -> + (for both)
- Joystick 1:
   - pin vrx -> A0 arduino (Outputs analog voltage based on horizontal joystick movement)
   - pin vry -> A1 Arduino (Outputs analog voltage based on vertical joystick movement)
   - pin SW -> 8 Arduino (digital) (Outputs digital signal when the joystick is pressed down)

- Joystick 2:
   - pin vrx -> A2 Arduino
   - pin vry -> A3 Arduino
   - pin SW -> 9 Arduino (digital)
  
3. LED matrix
- pin vcc -> 5V
- pin gnd -> gnd
- pin DIN -> 10 Arduino (digital) (Transfers data from Arduino to the LED matrix.)
- pin CS -> 11 Arduino (digital)  (Chip select)
- pin CLK -> 12 Arduino (digital) (for data transfer synchronisation)

4. Buzzer
- Positive pin -> paired with a 330 ohm resistor
- Negative pin -> 13 Arduino (digital)
- middle pin potentiometer -> A4 Arduino
- left pin potentiometer -> gnd | righ pin potentiometer -> 5V


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
The software is built using distinct sections for:

- **Initialization**: Configuring hardware components and displaying the welcome screen.
- **Game Menu**: Managing menu options and user interactions.
- **Game Logic**: Handling paddle movement, ball physics, and scoring.

## Functionalities/Main functions
1. setup()
Purpose: Initializes the hardware components and displays the welcome screen.
Key Actions:
- Sets up the LCD, LED matrix, and joystick pins.
- Displays "P" on the LED matrix using displayP().
- Plays the startup song using playSong().
- Transitions to the main menu using displayMenu().


2. loop()
Purpose: Main program loop that manages the game state and input handling.
Key Actions:
- Handles joystick input using handleJoystick().
- Manages transitions between menu options, about section, how-to-play section, and game logic.
- Tracks timing for menu interactions, about, and how-to-play displays.

3. playSong()
Purpose: Plays a short melody during the startup sequence.
Key Actions:
- Plays four notes sequentially using the tone() function.
- Prevents static noise by setting the buzzer pin to input after playing.

4. displayP()
Purpose: Displays the letter "P" on the LED matrix during the startup screen.
Key Actions:
- Defines a byte array for the letter "P" and updates the LED matrix rows.

5. displayMenu()
Purpose: Displays the main menu options on the LCD.
Key Actions:
- Shows the currently selected menu option and the next option (if available).


6. displayDifficultyMenu()
Purpose: Displays the difficulty menu on the LCD.
Key Actions:
- Shows the current difficulty level and the next option (if available).


7. handleJoystick()
Purpose: Manages joystick input for navigating menus and selecting options.
Key Actions:
- Detects joystick movement to scroll through menu options.
- Detects button presses to select menu options and handle submenu navigation.
- Plays a short tone when scrolling through options.
- Handles transitions to game start, difficulty menu, about section, and how-to-play section.


8. startGameCountdown()
Purpose: Displays a countdown before starting the game.
Key Actions:
- Displays numbers "3", "2", "1", followed by "GO!" on the LCD.
- Calls displayPaddlesAndBall() to start the game.


9. displayPaddlesAndBall()
Purpose: Implements the main game logic for Pong.
Key Actions:
- Manages ball movement and collision detection with walls and paddles.
- Updates paddle positions based on joystick input.
- Tracks scores for both players.
- Ends the game and declares a winner when one player reaches a score of 5.
- Displays the paddles, ball, and scores on the LED matrix and LCD.


10. displayMenu()
Purpose: Displays the main menu options on the LCD.
Key Actions:
- Shows the currently selected menu option and the next option (if available).
## Results and conclusions

## Journal


