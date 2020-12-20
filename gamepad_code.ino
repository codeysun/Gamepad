#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// matrix settings
#define ROW_SIZE 5
#define COL_SIZE 6
#define R0 4
#define R1 5
#define R2 6
#define R3 7
#define R4 8
#define C0 21
#define C1 20
#define C2 19
#define C3 18
#define C4 15
#define C5 14

#define MOD_ROW R3
#define MOD_COL C0

// Matrix
byte cols[] = {C0, C1, C2, C3, C4, C5};
byte rows[] = {R0, R1, R2, R3, R4};
char matrix[ROW_SIZE][COL_SIZE] = {
  {KEY_ESC, '1', '2', '3', '4', '5'},
  {KEY_TAB, 'q', 'w', 'e', 'r', 't'},
  {0 , 'a', 's', 'd', 'f', 'g'},
  {KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b'},
  {0, 0, 0, KEY_LEFT_CTRL, KEY_LEFT_ALT, ' '}
};
char matrixMod[ROW_SIZE][COL_SIZE] = { //modifier key
  {KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5},
  {KEY_TAB, 'q', 'w', KEY_UP_ARROW, 'r', 'y'},
  {0 , 'a', KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, 'h'},
  {KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b'},
  {0, 0, 0, ',', '.', ' '}
};
bool matrixState[ROW_SIZE][COL_SIZE] = {0}; // 1 is pressed, 0 if not


void setup() {
  // init column inputs
  for (byte c = 0; c < COL_SIZE; ++c) {
    pinMode(cols[c], INPUT_PULLUP);
  }
  //init row outputs
  for (byte r = 0; r < ROW_SIZE; ++r) {
    pinMode(rows[r], OUTPUT);
    digitalWrite(rows[r], HIGH);
    
  }
  Serial.begin(9600);
  Keyboard.begin();
  Serial.println("Running");
}

void loop() {
  for (byte r = 0; r < ROW_SIZE; ++r) {
    digitalWrite(rows[r], LOW);
    delayMicroseconds(5);
    
    for (byte c = 0; c < COL_SIZE; ++c) { //traverse cols
      if (digitalRead(cols[c]) == LOW) {
        keyPress(r, c);
      } else if (matrixState[r][c]) {
        keyRelease(r, c);
      }
    }
    
    digitalWrite(rows[r], HIGH);
    delayMicroseconds(5);
  }
}

void keyPress(int r, int c) {
  if (matrixState[MOD_ROW][MOD_COL]) {
    Keyboard.press(matrixMod[r][c]);
  } else {
    Keyboard.press(matrix[r][c]);
  }
  matrixState[r][c] = 1;
}

void keyRelease(int r, int c) {
  if (r == MOD_ROW && c == MOD_COL) {
    Keyboard.releaseAll();
  } else if (matrixState[MOD_ROW][MOD_COL]) {
    Keyboard.release(matrixMod[r][c]);
  } else {
    Keyboard.release(matrix[r][c]);
  }
  matrixState[r][c] = 0;
}
