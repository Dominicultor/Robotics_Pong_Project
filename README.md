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
1. **setup()**
Purpose: Initializes the hardware components and displays the welcome screen.
Key Actions:
- Sets up the LCD, LED matrix, and joystick pins.
- Displays "P" on the LED matrix using displayP().
- Plays the startup song using playSong().
- Transitions to the main menu using displayMenu().
```cpp
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Pong");

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(buzzerPin, OUTPUT);
  pinMode(joystickSW, INPUT_PULLUP);

  displayP(); // Display "P" during the startup
  unsigned long startMillis = millis();
  playSong(); // Play startup song

  // Wait for 2 seconds total
  while (millis() - startMillis < 2000) {
    // Do nothing; just wait
  }

  // Clear the LED matrix after 2 seconds
  lc.clearDisplay(0);

  // Transition directly to the menu
  lcd.clear();
  inMenu = true;
  displayMenu();
}
 ```

2. **loop()**
Purpose: Main program loop that manages the game state and input handling.
Key Actions:
- Handles joystick input using handleJoystick().
- Manages transitions between menu options, about section, how-to-play section, and game logic.
- Tracks timing for menu interactions, about, and how-to-play displays.
```cpp
void loop() {
  unsigned long currentMillis = millis();

  if (inMenu && !displayingSelection && !inAboutSection && !inHowToPlaySection) {
    handleJoystick();
  }

  if (displayingSelection && currentMillis >= displayEndMillis) {
    displayingSelection = false;
    if (inDifficultyMenu) {
      displayDifficultyMenu();
    } else {
      displayMenu();
    }
  }

  if (inAboutSection) {
    if (!aboutSecondMessageDisplayed && currentMillis - aboutStartMillis >= 2000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mihailescu");
      lcd.setCursor(0, 1);
      lcd.print("Dominic");
      aboutSecondMessageDisplayed = true;
    } else if (aboutSecondMessageDisplayed && currentMillis - aboutStartMillis >= 4000) {
      inAboutSection = false;
      displayMenu();
    }
  }

  if (inHowToPlaySection) {
    if (!howToPlaySecondMessageDisplayed && currentMillis - howToPlayStartMillis >= 2000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("First to 5");
      lcd.setCursor(0, 1);
      lcd.print("wins the game!");
      howToPlaySecondMessageDisplayed = true;
    } else if (howToPlaySecondMessageDisplayed && currentMillis - howToPlayStartMillis >= 4000) {
      inHowToPlaySection = false;
      displayMenu();
    }
  }
}
```

3. **playSong()**
Purpose: Plays a short melody during the startup sequence.
Key Actions:
- Plays four notes sequentially using the tone() function.
- Prevents static noise by setting the buzzer pin to input after playing.

```cpp
void playSong() {
  int melody[] = {262, 330, 392, 523}; // Notes for the startup song
  int noteDuration = 500;                  // Duration of each note

  for (int i = 0; i < 4; i++) {
    tone(buzzerPin, melody[i], noteDuration);
    delay(noteDuration);
  }
  noTone(buzzerPin);           // Stop the buzzer
  pinMode(buzzerPin, INPUT);   // Prevent static noise
}
```
4. **displayP()**
Purpose: Displays the letter "P" on the LED matrix during the startup screen.
Key Actions:
- Defines a byte array for the letter "P" and updates the LED matrix rows.

```cpp
void displayP() {
  byte letterP[8] = {
    B11111100,
    B10000010,
    B11111100,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B00000000
  };

  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, letterP[row]);
  }
}
```
5. **displayMenu()**
Purpose: Displays the main menu options on the LCD.
Key Actions:
- Shows the currently selected menu option and the next option (if available).

```cpp
void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + menuOptions[currentSelection]);
  if (currentSelection + 1 < menuCount) {
    lcd.setCursor(0, 1);
    lcd.print(menuOptions[currentSelection + 1]);
  }
}
```
6. **displayDifficultyMenu()**
Purpose: Displays the difficulty menu on the LCD.
Key Actions:
- Shows the current difficulty level and the next option (if available).
```cpp
void displayDifficultyMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + difficultyOptions[currentDifficulty]);
  if (currentDifficulty + 1 < difficultyCount) {
    lcd.setCursor(0, 1);
    lcd.print(difficultyOptions[currentDifficulty + 1]);
  }
}
```

7. **handleJoystick()**
Purpose: Manages joystick input for navigating menus and selecting options.
Key Actions:
- Detects joystick movement to scroll through menu options.
- Detects button presses to select menu options and handle submenu navigation.
- Plays a short tone when scrolling through options.
- Handles transitions to game start, difficulty menu, about section, and how-to-play section.

```cpp
void handleJoystick() {
  int vry = analogRead(joystickVRy);
  int buttonState = digitalRead(joystickSW);
  unsigned long currentMillis = millis();

  if (currentMillis - lastMoveTime > moveInterval) {
    if (vry < 400) { // Joystick moved up
      if (inDifficultyMenu && currentDifficulty > 0) {
        currentDifficulty--;
        displayDifficultyMenu();
      } else if (!inDifficultyMenu && currentSelection > 0) {
        currentSelection--;
        displayMenu();
      }
      lastMoveTime = currentMillis;
      tone(buzzerPin, 1000);               // Start scroll sound
      lastBuzzResetTime = currentMillis + 50; // Schedule buzzer reset
      buzzActive = true;
    } else if (vry > 600) { // Joystick moved down
      if (inDifficultyMenu && currentDifficulty < difficultyCount - 1) {
        currentDifficulty++;
        displayDifficultyMenu();
      } else if (!inDifficultyMenu && currentSelection < menuCount - 1) {
        currentSelection++;
        displayMenu();
      }
      lastMoveTime = currentMillis;
      tone(buzzerPin, 1000);               // Start scroll sound
      lastBuzzResetTime = currentMillis + 50; // Schedule buzzer reset
      buzzActive = true;
    }
  }

  if (buzzActive && currentMillis >= lastBuzzResetTime) {
    noTone(buzzerPin);         // Stop the tone
    pinMode(buzzerPin, INPUT); // Set buzzer pin to INPUT to avoid static noise
    buzzActive = false;
  }

  if (buttonState == LOW && currentMillis - lastButtonPress > debounceTime) {
    lastButtonPress = currentMillis;
    if (!inDifficultyMenu && menuOptions[currentSelection] == "Start Game") {
      startGameCountdown(); // Display countdown when "Start Game" is selected
    } else if (!inDifficultyMenu && menuOptions[currentSelection] == "Difficulty") {
      inDifficultyMenu = true;
      displayDifficultyMenu();
    } else if (!inDifficultyMenu && menuOptions[currentSelection] == "About") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PONG");
      lcd.setCursor(0, 1);
      lcd.print("By Dominicultor");
      aboutStartMillis = currentMillis;
      inAboutSection = true;
      aboutSecondMessageDisplayed = false;
    } else if (!inDifficultyMenu && menuOptions[currentSelection] == "How to Play") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Use joystick to");
      lcd.setCursor(0, 1);
      lcd.print("move paddles.");
      howToPlayStartMillis = currentMillis;
      inHowToPlaySection = true;
      howToPlaySecondMessageDisplayed = false;
    } else if (inDifficultyMenu) {
      lcd.clear();
      lcd.print("Interval: ");
      if (difficultyOptions[currentDifficulty] == "Easy") {
        ballMoveInterval = 2000;
        lcd.print("2s");
      } else if (difficultyOptions[currentDifficulty] == "Medium") {
        ballMoveInterval = 1000;
        lcd.print("1s");
      } else if (difficultyOptions[currentDifficulty] == "Hard") {
        ballMoveInterval = 500;
        lcd.print("0.5s");
      }
      inDifficultyMenu = false;
      displayEndMillis = currentMillis + 2000; // Display interval for 2 seconds
      displayingSelection = true;
    }
  }
}
```
8. **startGameCountdown()**
Purpose: Displays a countdown before starting the game.
Key Actions:
- Displays numbers "3", "2", "1", followed by "GO!" on the LCD.
- Calls displayPaddlesAndBall() to start the game.

```cpp 
void startGameCountdown() {
  lcd.clear();
  for (int i = 3; i > 0; i--) {
    lcd.setCursor(0, 0);
    lcd.print(i);
    delay(1000);
  }
  lcd.setCursor(0, 0);
  lcd.print("GO!");
  delay(1000); // Display "GO!" momentarily

  // Start the game logic after countdown
  displayPaddlesAndBall();
}
```
9. **displayPaddlesAndBall()**
Purpose: Implements the main game logic for Pong.
Key Actions:
- Manages ball movement and collision detection with walls and paddles.
- Updates paddle positions based on joystick input.
- Tracks scores for both players.
- Ends the game and declares a winner when one player reaches a score of 5.
- Displays the paddles, ball, and scores on the LED matrix and LCD.

```cpp 
void displayPaddlesAndBall() {
  int ballX = 3, ballY = 3;                // Ball position
  int ballDirectionX = 1, ballDirectionY = 1; // Ball direction
  int leftPaddleY = 3;                     // Left paddle position
  int rightPaddleY = 3;                    // Right paddle position
  unsigned long lastBallMoveTime = 0;      // Timing for ball movement
  unsigned long paddleMoveTime = 0;        // Timing for paddle movement
  int leftScore = 0, rightScore = 0;       // Player scores

  // Define the pins for the second joystick
  const int joystick2VRx = A2; // Right joystick VRx (horizontal)
  const int joystick2VRy = A3; // Right joystick VRy (vertical)
  const int joystick2SW = 9;   // Right joystick button (digital)

  pinMode(joystick2SW, INPUT_PULLUP); // Set the second joystick button as input

  while (true) {
    unsigned long currentMillis = millis();

    // Ball movement
    if (currentMillis - lastBallMoveTime > ballMoveInterval) {
      // Clear previous ball position
      lc.setLed(0, ballY, ballX, false);

      // Update ball position
      ballX += ballDirectionX;
      ballY += ballDirectionY;

      // Check for wall collisions
      if (ballY <= 0 || ballY >= 7) {
        ballDirectionY = -ballDirectionY;
      }

      // Check for paddle collisions
      if (ballX == 1 && ballY >= leftPaddleY && ballY <= leftPaddleY + 2) {
        ballDirectionX = 1;
        tone(buzzerPin, 1000, 50); // Paddle hit sound
        pinMode(buzzerPin, INPUT); 

      } else if (ballX == 6 && ballY >= rightPaddleY && ballY <= rightPaddleY + 2) {
        ballDirectionX = -1;
        tone(buzzerPin, 1000, 50); // Paddle hit sound
        pinMode(buzzerPin, INPUT); 
      }

      // Check for scoring
      if (ballX < 0) { // Ball passed left paddle
        rightScore++;
        lcd.clear();
        lcd.print("Right Scores!");
        delay(1000);
        if (rightScore == 5) { // Check win condition
          lcd.clear();
          lcd.print("Right Wins!");
          delay(3000);
          break; // Exit the game loop
        }
        ballX = 3;
        ballY = 3;
        ballDirectionX = 1;
        continue;
      } else if (ballX > 7) { // Ball passed right paddle
        leftScore++;
        lcd.clear();
        lcd.print("Left Scores!");
        delay(1000);
        if (leftScore == 5) { // Check win condition
          lcd.clear();
          lcd.print("Left Wins!");
          delay(3000);
          break; // Exit the game loop
        }
        ballX = 3;
        ballY = 3;
        ballDirectionX = -1;
        continue;
      }

      // Draw updated ball position
      lc.setLed(0, ballY, ballX, true);
      lastBallMoveTime = currentMillis;
    }

    // Paddle movement
    if (currentMillis - paddleMoveTime > 100) {
      // Joystick input for left paddle
      int vry = analogRead(joystickVRy);
      if (vry < 400 && leftPaddleY > 0) {
        leftPaddleY--;
      } else if (vry > 600 && leftPaddleY < 5) {
        leftPaddleY++;
      }

      // Joystick input for right paddle
      int vry2 = analogRead(joystick2VRy);
      if (vry2 < 400 && rightPaddleY > 0) {
        rightPaddleY--;
      } else if (vry2 > 600 && rightPaddleY < 5) {
        rightPaddleY++;
      }

      paddleMoveTime = currentMillis;
    }

    // Update paddles and ball on display
    lc.clearDisplay(0);

    // Draw left paddle
    for (int i = 0; i < 3; i++) {
      lc.setLed(0, leftPaddleY + i, 0, true);
    }

    // Draw right paddle
    for (int i = 0; i < 3; i++) {
      lc.setLed(0, rightPaddleY + i, 7, true);
    }

    // Draw ball
    lc.setLed(0, ballY, ballX, true);

    // Update scores on LCD
    lcd.setCursor(0, 0);
    lcd.print("L: ");
    lcd.print(leftScore);
    lcd.setCursor(8, 0);
    lcd.print("R: ");
    lcd.print(rightScore);
  }

  // Game Over: Return to menu or restart logic
  inMenu = true; // Return to the main menu
  displayMenu();
}
```


[Watch video here!]([https://youtu.be/372jr29r4Q4](https://youtube.com/shorts/5vbemcqpsYs)


## Results and conclusions

## Journal


