/**
 * Open Beer Filler
 * Copyright (c) 2020 Gerhard Potgieter [https://gerhardpotgieter.com]
 *
 * Based on an idea by Christopher Harrison-Hawkes [https://harrisonsbrewery.com]
 * Modified By David Gray (https://github.com/N3MIS15)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <Servo.h>
#include "InputConfig.h"
#include "ReprapLCD.h"
#include "Move.h"
#include "Settings.h"

bool triggerState = false;
bool idleMessageDisplayed = false;
enum ProgramState {UNDEF, IDLE, START, PRE_PURGE, FILLING, POST_PURGE, STOP};
char ProgramStateText[][8] = {"UNDEF", "Idle", "Feeding", "Purging", "Filling", "Purging",  "Stop"};
ProgramState currentState = UNDEF;
char statusOutput[21];
int cansFilled = 0;
Servo fillServo;

/**
 * ***************************************************************************
 * ******************************** FUNCTIONS ********************************
 * ***************************************************************************
 */


void setupPins() {
  pinMode(CO2_PURGE_SOL, OUTPUT);
  pinMode(END_STOP, INPUT);
  pinMode(BEER_FILL_SENSOR, INPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);
  fillServo.attach(SERVO_SIGNAL);
}


void checkFillSensor() {
  if (EEPROM16_Read(EEPROM_FILL_SENSOR_TRIGGER) < analogRead(BEER_FILL_SENSOR)) {
    Serial.println("Fill sensor triggered");
    triggerFullFillSensor();
  }
}


void triggerFullFillSensor() {
  if (!triggerState && hasProgramState(FILLING)) {
    closeBeerFillerTube();
    triggerState = true;
    Serial.println("Filler tube closed");
  }
}


void closeBeerFillerTube() {
  fillServo.write(EEPROM16_Read(EEPROM_SERVO_POS_IDLE));
}


void openBeerFillerTube() {
  Serial.println("Opening beer filler tube");
  fillServo.write(EEPROM16_Read(EEPROM_SERVO_POS_FILL));
}


void purgeCO2( bool retract = false ) {
  Serial.println("Purging CO2");
  sprintf(statusOutput, "%s%s", "Status: ", "Purging");
  updateLine(0, statusOutput);
  updateLCD();

  if(!retract) {
    digitalWrite(CO2_PURGE_SOL, HIGH);
    delay(EEPROM16_Read(EEPROM_CO2_PRE_PURGE_PERIOD));
  } else {
    if (EEPROM16_Read(EEPROM_CO2_POST_PURGE_PERIOD)) {
      digitalWrite(CO2_PURGE_SOL, HIGH);
      delay(EEPROM16_Read(EEPROM_CO2_POST_PURGE_PERIOD));
    }
  }
  digitalWrite(CO2_PURGE_SOL, LOW);
}


void raiseFillerTube() {
  Serial.println("Raising filler tube");
  moveHome(); 
}


void idleState() {
  if (!idleMessageDisplayed) {
    Serial.println("Press Start Button to proceed");
    idleMessageDisplayed = true;
    moveHome();
  }
  readStartButton();
}


void startState() {
  movePrePurge();
  changeProgramState(PRE_PURGE);
}


void prePurgeState() {
  Serial.println("Purging CO2");
  digitalWrite(CO2_PURGE_SOL, HIGH);
  delay(EEPROM16_Read(EEPROM_CO2_PRE_PURGE_PERIOD));
  digitalWrite(CO2_PURGE_SOL, LOW);
  moveFill();
  openBeerFillerTube();
  changeProgramState(FILLING);
}


void fillingState() {
  // Check if we are done filling.
  if(triggerState){
    cansFilled++;
    triggerState = false;
    if (EEPROM16_Read(EEPROM_CO2_POST_PURGE_PERIOD)) {
      movePostPurge();
      changeProgramState(POST_PURGE);
    } else {
      moveHome();
      changeProgramState(IDLE);
    }
  }
}


void postPurgeState() {
  Serial.println("Purging CO2");
  digitalWrite(CO2_PURGE_SOL, HIGH);
  delay(EEPROM16_Read(EEPROM_CO2_POST_PURGE_PERIOD));
  digitalWrite(CO2_PURGE_SOL, LOW);
  raiseFillerTube();
  changeProgramState(IDLE);
}


void stopState() {
  // Reset the sensors and change ProgramState to IDLE.
  resetUnit();
  changeProgramState(IDLE);
}


void readStartButton() {
  if(LOW==digitalRead(START_BUTTON) && hasProgramState(IDLE)) {
    Serial.println("Start Button Pressed");
    changeProgramState(START);
  }
}


void readStopButton() {
  if(
    LOW==digitalRead(START_BUTTON)
    && !hasProgramState(IDLE)
    && !hasProgramState(START)
  ) {
    Serial.println("Stop Button Pressed");
    changeProgramState(STOP);
  }
}


void resetUnit() {
  Serial.println("Resetting unit");
  closeBeerFillerTube();
  raiseFillerTube();
  digitalWrite(CO2_PURGE_SOL, LOW);
  Serial.println("Done resetting unit");
  changeProgramState(IDLE);
}


void changeProgramState(ProgramState state) {
  if (IDLE == state){
    idleMessageDisplayed = false;
  }
  currentState = state;
  Serial.print("Program state changed: ");
  Serial.println(ProgramStateText[currentState]);
}


bool hasProgramState(ProgramState state) {
  if(state == currentState) {
    return true;
  }
  return false;
}


void alwaysRun() {
  readStopButton();
  checkFillSensor();
  rotEncRead();
  showDisplay(ProgramStateText[currentState], cansFilled);
  updateLCD();
}


/**
 * ***************************************************************************
 * ***************************** MAIN FUNCTIONS ******************************
 * ***************************************************************************
 */


void setup() {
  Serial.begin(115200);
  firstRunSettings();
  setupLCD();
  updateLCD();
  setupPins();
  setupMovement();
  fillServo.write(EEPROM16_Read(EEPROM_SERVO_POS_IDLE));
  resetUnit();
}


void loop() {
  switch(currentState) {
    case IDLE:
      idleState();
      break;
    case START:
      startState();
      break;
    case PRE_PURGE:
      prePurgeState();
      break;
    case FILLING:
      fillingState();
      break;
    case POST_PURGE:
      postPurgeState();
      break;
    case STOP:
      stopState();
      break;
  }
  alwaysRun();
}
