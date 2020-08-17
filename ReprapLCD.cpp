
#pragma once
#include "ReprapLCD.h"

/* Setup 2040 LCD */
LiquidCrystal_I2C lcd(0x27, 20, 4);
char lines[4][21];

/* Setup Rotary Encoder */
#define ENCODER_OPTIMIZE_INTERRUPTS
Encoder rotEnc(ROT_ENC_A, ROT_ENC_B);
int oldPosition, newPosition;
bool initializing = true;
int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

/* Setup Menu Stuff */
int menuScreenPosition, cursorPos = 0;
enum displayPage {HOME,MENU,VALUE};
displayPage currentPage = HOME;
enum menuSelect {
                  INFO, 
                  FILL_TRIG, 
                  PRE_PURGE_TIME, 
                  POST_PURGE_TIME, 
                  FILL_POS_STEPS, 
                  PRE_PURGE_POS,
                  POST_PURGE_POS, 
                  SERVO_IDLE_POS, 
                  SERVO_FILL_POS
};
menuSelect currentSelection = INFO;
char menuListText[][21] = {
                  "Info Screen", 
                  "Fill Trigger", 
                  "Pre Purge Time", 
                  "Post Purge Time", 
                  "Fill Position", 
                  "Pre Purge Position", 
                  "Post Purge Position", 
                  "Servo Idle Position", 
                  "Servo Fill Position"
};
int menuListLength = ((sizeof(menuListText))/(sizeof(menuListText[0])));
int valueBuffer;
bool fromMenu;


void updateLine(int line, char output[]) {
  sprintf(lines[line], "%-20s", output);
}


void updateLCD() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0,i);
    lcd.print(lines[i]);
  }
}


void setupLCD() {
  lcd.init();
  lcd.backlight();
  updateLine(0, "Initialising...");
  pinMode(ROT_ENC_BUTTON, INPUT);
  initializing = false;
}


void showDisplay(char currentStateText[], int filledCans) {
  char lineText[4][21] = {" ", " ", " ", " "};

  switch(currentPage) {
    case INFO: 
      sprintf(lineText[0], "%s%s", "Status: ", currentStateText);
      sprintf(lineText[1], "%s%i", "Cans Filled: ", filledCans);
      break;

    case MENU:
      cursorPos = currentSelection - menuScreenPosition;
      lineText[cursorPos][0] = 62; // >

      sprintf(lineText[0], "%s%s", lineText[0], menuListText[menuScreenPosition]);
      sprintf(lineText[1], "%s%s", lineText[1], menuListText[menuScreenPosition+1]);
      sprintf(lineText[2], "%s%s", lineText[2], menuListText[menuScreenPosition+2]);
      sprintf(lineText[3], "%s%s", lineText[3], menuListText[menuScreenPosition+3]);
      break;

    case VALUE:
      switch (currentSelection) {
        case FILL_TRIG:
          sprintf(lineText[0], "%s", "Fill Trigger");
          sprintf(lineText[2], "%s%i", "Trigger value: ", valueBuffer);
          break;
        case PRE_PURGE_TIME:
          sprintf(lineText[0], "%s", "Pre Purge Time");
          sprintf(lineText[2], "%s%i", "Time in ms: ", valueBuffer);
          break;
        case POST_PURGE_TIME:
          sprintf(lineText[0], "%s", "Post Purge Time");
          sprintf(lineText[2], "%s%i", "Time in ms: ", valueBuffer);
          break;
        case FILL_POS_STEPS:
          sprintf(lineText[0], "%s", "Fill Position");
          sprintf(lineText[2], "%s%i", "Steps from 0: ", valueBuffer);
          break;
        case PRE_PURGE_POS:
          sprintf(lineText[0], "%s", "Pre Purge Position");
          sprintf(lineText[2], "%s%i", "Steps from 0: ", valueBuffer);
          break;
        case POST_PURGE_POS:
          sprintf(lineText[0], "%s", "Post Purge Position");
          sprintf(lineText[2], "%s%i", "Steps from 0: ", valueBuffer);
          break;
        case SERVO_IDLE_POS:
          sprintf(lineText[0], "%s", "Servo at Idle");
          sprintf(lineText[2], "%s%i", "Position: ", valueBuffer);
          break;
        case SERVO_FILL_POS:
          sprintf(lineText[0], "%s", "Position at Fill");
          sprintf(lineText[2], "%s%i", "Position: ", valueBuffer);
          break;
      }
      break;
  }

  updateLine(0, lineText[0]);
  updateLine(1, lineText[1]);
  updateLine(2, lineText[2]);
  updateLine(3, lineText[3]);
}


int valueLowHigh(int rotEncncPosition, int lowVal, int highVal) {
  if (rotEncncPosition < (lowVal+1))    { rotEncncPosition = lowVal;  rotEnc.write(lowVal); }
  if (rotEncncPosition > (highVal-1))   { rotEncncPosition = highVal; rotEnc.write(highVal); }
  return rotEncncPosition;
}


void readRotEncRotation() {
  newPosition = rotEnc.read() / 4;
  switch(currentPage) {
    case HOME:
      break;

    case MENU:
      if (newPosition < 1) { newPosition = 0; rotEnc.write(0);}
      if (newPosition > (menuListLength-2)) { newPosition = menuListLength-1; rotEnc.write(newPosition * 4);}

      // check if we need to scroll the screen
      // Cursor Moved Down
      if (newPosition > currentSelection) {
        if ((newPosition-3) > menuScreenPosition) {
          menuScreenPosition++;
        }
      }
      
      // Cursor Moved Up
      if (newPosition < menuScreenPosition) {
        menuScreenPosition--;
      }

      // Stop the cursor going too far
      if (newPosition > (menuScreenPosition + 3)) {newPosition = menuScreenPosition + 3;}
      currentSelection = newPosition;
      break;

     case VALUE:
      int x;
      if (fromMenu) {
        newPosition = valueBuffer;
        fromMenu = false;
      }
      
       switch(currentSelection) {
        case FILL_TRIG:
          x = valueLowHigh(newPosition, 0, 1023);
          break;
        case PRE_PURGE_TIME:
          x = valueLowHigh(newPosition, 0, 32767);
          break;
        case POST_PURGE_TIME:
          x = valueLowHigh(newPosition, 0, 32767);
          break;
        case FILL_POS_STEPS:
          x = valueLowHigh(newPosition, 0, 32767);
          break;
        case PRE_PURGE_POS:
          x = valueLowHigh(newPosition, 0, 32767);
          break;
        case POST_PURGE_POS:
          x = valueLowHigh(newPosition, 0, 32767);
          break;
        case SERVO_IDLE_POS:
          x = valueLowHigh(newPosition, 0, 180);
          break;
        case SERVO_FILL_POS:
          x = valueLowHigh(newPosition, 0, 180);
          break;
      }
      valueBuffer = x;
      break;
  }
}


void readRotEncButton() {
  int reading = digitalRead(ROT_ENC_BUTTON);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle if the button state is LOW
      if (buttonState == LOW) {

        switch(currentPage) {
          case HOME:
            currentPage = MENU;
            currentSelection = INFO;
            rotEnc.write(0);
            break;

          case MENU:
            fromMenu = true;
            valueBuffer = EEPROM16_Read(currentSelection*10);
            rotEnc.write(valueBuffer*4);

            switch(currentSelection) {
              case INFO:
                currentPage = HOME;
                break;
              case FILL_TRIG:
                currentPage = VALUE;
                break;
              case PRE_PURGE_TIME:
                currentPage = VALUE;
                break;
              case POST_PURGE_TIME:
                currentPage = VALUE;
                break;
              case FILL_POS_STEPS:
                currentPage = VALUE;
                break;
              case PRE_PURGE_POS:
                currentPage = VALUE;
                break;
              case POST_PURGE_POS:
                currentPage = VALUE;
                break;
              case SERVO_IDLE_POS:
                currentPage = VALUE;
                break;
              case SERVO_FILL_POS:
                currentPage = VALUE;
                break;
            }
            break;

          case VALUE:
            EEPROM16_Write(currentSelection*10, valueBuffer);
            newPosition = currentSelection;
            rotEnc.write(currentSelection*4);
            currentPage = MENU;
            break;
        }
      }
    }
  }

  lastButtonState = reading;
}


void rotEncRead() {
  readRotEncButton();
  readRotEncRotation();
}
