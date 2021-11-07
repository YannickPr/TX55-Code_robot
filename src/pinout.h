#ifndef PINOUT_H_
#define PINOUT_H_
#include <Arduino.h>

/*
 * pinout of CNC shield UNO
 *
 * source: http://reprap.org/wiki/RAMPS_1.4
 */


#define ENABLE_PIN         8

#define X_STEP_PIN         2
#define X_DIR_PIN          5
#define X_MIN_PIN               3
#define X_MAX_PIN               2

#define Y_STEP_PIN         3
#define Y_DIR_PIN          6
#define Y_MIN_PIN           14
#define Y_MAX_PIN           15

#define Z_STEP_PIN         4
#define Z_DIR_PIN          7
#define Z_MIN_PIN           18
#define Z_MAX_PIN           19

#define E_STEP_PIN         12
#define E_DIR_PIN          13


#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN             9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN        8
#define TEMP_0_PIN         13   // ANALOG NUMBERING
#define TEMP_1_PIN         14   // ANALOG NUMBERING

//RAMPS AUX-1
#define STEPPER_GRIPPER_PIN_0 40
#define STEPPER_GRIPPER_PIN_1 59
#define STEPPER_GRIPPER_PIN_2 63
#define STEPPER_GRIPPER_PIN_3 64



#endif
