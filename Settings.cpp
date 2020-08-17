
#pragma once
#include "Settings.h"

int settingsVersion = 1;


void EEPROM16_Write(uint8_t a, uint16_t b) {
  Serial.println("Writing to EEPROM");
  EEPROM.write(a, lowByte(b));
  EEPROM.write(a+1, highByte(b));
}


uint16_t EEPROM16_Read(uint8_t a) {
  return word(EEPROM.read(a+1), EEPROM.read(a));
}


void firstRunSettings() {
  // If EEPROM position 0 has not been set to 1, assume first run
  if (!(EEPROM16_Read(0) == settingsVersion)) { 

    // Write defaults to EEPROM
    EEPROM16_Write(EEPROM_FILL_SENSOR_TRIGGER, FILL_SENSOR_TRIGGER);
    EEPROM16_Write(EEPROM_CO2_PRE_PURGE_PERIOD, CO2_PRE_PURGE_PERIOD);
    EEPROM16_Write(EEPROM_CO2_POST_PURGE_PERIOD, CO2_POST_PURGE_PERIOD);
    EEPROM16_Write(EEPROM_FILL_POSITION, FILL_POSITION);
    EEPROM16_Write(EEPROM_CO2_PRE_PURGE_POSITION, CO2_PRE_PURGE_POSITION);
    EEPROM16_Write(EEPROM_CO2_POST_PURGE_POSITION, CO2_POST_PURGE_POSITION);
    EEPROM16_Write(EEPROM_SERVO_POS_IDLE, SERVO_POS_IDLE);
    EEPROM16_Write(EEPROM_SERVO_POS_FILL, SERVO_POS_FILL);
    EEPROM16_Write(0, settingsVersion);
  }
}
