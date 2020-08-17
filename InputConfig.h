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
#include <Arduino.h>
/**
 * Pin definitions
 */
#define BEER_INLET_SOL    7
#define BEER_FILL_SENSOR  A0
#define START_BUTTON      10
#define CO2_PURGE_SOL     6
#define SERVO_SIGNAL      4
#define ROT_ENC_A         3
#define ROT_ENC_B         2
#define ROT_ENC_BUTTON    12
#define STEPPER_STEP      8
#define STEPPER_DIR       9
#define END_STOP          11
