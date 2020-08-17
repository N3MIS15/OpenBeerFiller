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

/**
 * General Config Options
 */
 
#define FILL_SENSOR_TRIGGER     400   // Ints between 0 and 1023 used to trigger the fill sensor: operating voltage(5v or 3.3v) / 1024
#define CO2_PRE_PURGE_PERIOD    3000  // Time in ms
#define CO2_POST_PURGE_PERIOD   0     // Time in ms, use zero to skip this step
#define FILL_POSITION           1610  // Number of steps from home
#define CO2_PRE_PURGE_POSITION  1550  // Number of steps from home
#define CO2_POST_PURGE_POSITION 760   // Number of steps from home
#define SERVO_POS_IDLE          0     // Angle of servo while not filling
#define SERVO_POS_FILL          60    // Angle of servo while filling
