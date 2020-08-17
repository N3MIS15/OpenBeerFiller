
#pragma once
#include "Move.h"


void setupMovement() {
  pinMode(STEPPER_STEP, OUTPUT);
  pinMode(STEPPER_DIR, OUTPUT);
}


void moveHome() {
  digitalWrite(STEPPER_DIR, LOW);

  while(digitalRead(END_STOP) == LOW) {
    digitalWrite(STEPPER_STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEPPER_STEP, LOW);
    delayMicroseconds(500);
  }
}


void moveFill() {
  digitalWrite(STEPPER_DIR, HIGH);

  for (int i = 0; i < (EEPROM16_Read(EEPROM_FILL_POSITION) - EEPROM16_Read(EEPROM_CO2_PRE_PURGE_POSITION)); i++) {
    digitalWrite(STEPPER_STEP, HIGH);
    delayMicroseconds(1250);
    digitalWrite(STEPPER_STEP, LOW);
    delayMicroseconds(1250);
  }
}


void movePrePurge() {
  digitalWrite(STEPPER_DIR, HIGH);

  for (int i = 0; i < EEPROM16_Read(EEPROM_CO2_PRE_PURGE_POSITION); i++) {
    digitalWrite(STEPPER_STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEPPER_STEP, LOW);
    delayMicroseconds(500);
  }
}


void movePostPurge() {
  digitalWrite(STEPPER_DIR, LOW);

  for (int i = 0; i < (EEPROM16_Read(EEPROM_FILL_POSITION) - EEPROM16_Read(EEPROM_CO2_POST_PURGE_POSITION)); i++) {
    digitalWrite(STEPPER_STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEPPER_STEP, LOW);
    delayMicroseconds(500);
  }
}
