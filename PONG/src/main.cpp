#include <LedControl.h>
#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD pin configuration
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

// Initialize the library with DIN, CLK, CS pins for LED Matrix
LedControl lc = LedControl(10, 12, 11, 1); // (DIN, CLK, CS, number of devices)

// Pin configuration for buzzer
const int buzzerPin = A5;  // Pin where the buzzer is connected (A5)

// Joystick pin configuration
const int joystickVRx = A0; // Joystick VRx (horizontal)
const int joystickVRy = A1; // Joystick VRy (vertical)
const int joystickSW = 8;   // Joystick button (digital)

// Welcome message timing
unsigned long previousMillis = 0;  // Stores the last time the message was updated
const long interval = 5000;        // Interval for the "Welcome" message (5 seconds)
bool welcomeDisplayed = false;     // Flag to check if the welcome message is displayed

// Menu options
const String menuOptions[] = {"Start Game", "Difficulty", "About", "How to Play"};
const int menuCount = sizeof(menuOptions) / sizeof(menuOptions[0]);
int currentSelection = 0; // Current selected menu item
bool inMenu = false;      // Flag to check if we are in the menu

// Difficulty submenu
const String difficultyOptions[] = {"Easy", "Medium", "Hard"};
const int difficultyCount = sizeof(difficultyOptions) / sizeof(difficultyOptions[0]);
int currentDifficulty = 0; // Current selected difficulty
bool inDifficultyMenu = false; // Flag for difficulty submenu
unsigned long ballMoveInterval = 2000; // Default ball move interval

// Timing variables for joystick debounce
unsigned long lastMoveTime = 0; // Last time the joystick was moved
const long moveInterval = 200;  // Time between movements

// Timing variables for button debounce and display
unsigned long lastButtonPress = 0;     // Last time the joystick button was pressed
const long debounceTime = 200;         // Debounce time for joystick button
unsigned long displayEndMillis = 0;    // Time to end the selected display
bool displayingSelection = false;      // Flag to track if selection display is active

unsigned long lastBuzzResetTime = 0; // Tracks when to reset the buzzer
bool buzzActive = false;            // Tracks if the buzzer is currently active

//About section variables
bool inAboutSection = false;
unsigned long aboutStartMillis = 0;
bool aboutSecondMessageDisplayed = false;

//How to Play section variables
unsigned long howToPlayStartMillis = 0; // Tracks when "How to Play" was accessed
bool inHowToPlaySection = false;       // Flag to indicate we're in the "How to Play" section
bool howToPlaySecondMessageDisplayed = false; // Flag for the second message


// Function prototypes
void displayP();            // Function to display "P" on the LED matrix
void playSong();            // Function to play the startup song
void displayMenu();         // Function to display the menu
void displayDifficultyMenu(); // Function to display the difficulty submenu
void handleJoystick();      // Function to handle joystick input
void startGameCountdown();  // Function to display countdown before game starts
void displayPaddlesAndBall(); // Function to display paddles and ball

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

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + menuOptions[currentSelection]);
  if (currentSelection + 1 < menuCount) {
    lcd.setCursor(0, 1);
    lcd.print(menuOptions[currentSelection + 1]);
  }
}

void displayDifficultyMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + difficultyOptions[currentDifficulty]);
  if (currentDifficulty + 1 < difficultyCount) {
    lcd.setCursor(0, 1);
    lcd.print(difficultyOptions[currentDifficulty + 1]);
  }
}

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

