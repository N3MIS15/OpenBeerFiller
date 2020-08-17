
#pragma once
#include <EEPROM.h>
#include "Config.h"
#include "InputConfig.h"

#define EEPROM_FILL_SENSOR_TRIGGER 10
#define EEPROM_CO2_PRE_PURGE_PERIOD 20
#define EEPROM_CO2_POST_PURGE_PERIOD 30
#define EEPROM_FILL_POSITION 40
#define EEPROM_CO2_PRE_PURGE_POSITION 50
#define EEPROM_CO2_POST_PURGE_POSITION 60
#define EEPROM_SERVO_POS_IDLE 70
#define EEPROM_SERVO_POS_FILL 80

void firstRunSettings();
void EEPROM16_Write(uint8_t a, uint16_t b);
uint16_t EEPROM16_Read(uint8_t a);
