/*
 * config.h
 *
 *  Created on: 20.09.2019
 *      Author: zwax
 */

#ifndef CONFIG_H_
#define CONFIG_H_


// scraper
// =============================================
#define SCRAPER_MICROSTEPS 2
#define PIN_STEPPER_SCRAPER_DIR 25
#define PIN_STEPPER_SCRAPER_STEP 26
#define PIN_STEPPER_SCRAPER_ENABLE 27
#define PIN_STEPPER_SCRAPER_ENDSTOP 24
#define SCRAPER_SPEED_FWD (500 * SCRAPER_MICROSTEPS)
#define SCRAPER_SPEED_REV (800 * SCRAPER_MICROSTEPS)
#define SCRAPER_ACCELARATION (2000 * SCRAPER_MICROSTEPS)
#define SCRAPER_DIST (3050 * SCRAPER_MICROSTEPS)
#define ROLLER_SPEED 50

// rozel
// =============================================
// encoder ticks per motor turn: 20
// gear ratio: 1:135
// ticks per deg: 20 * 135 / 360 = 2700 / 360 = 7.5
// #define ROZEL_TICKS_PER_DEG (5850/360.0) // woher kommt 5850???

// #define ROZEL_ENDPOS -2890
#define ROZEL_ENDPOS -1270
#define ROZEL_SPEED_UP 1000
#define ROZEL_SPEED_DOWN 1000

// display
// =============================================
#define DISPLAY_INTERVAL 500
#define TP_SETTING SPISettings(2000000, MSBFIRST, SPI_MODE0)
#define TP_CS 6
#define PIN_BACKLIGHT 9

// plate
// =============================================
#define PIN_PLATE_TEMP A9
#define PIN_PLATE_RELAY 22

// pneumatics
// =============================================
#define PRESSURE_SENSOR_SCALE_FACTOR 0.003225806F // 3.3 Volt / 1023
#define PRESSURE_SENSOR_OFFSET_VOLTAGE 0.57F
#define PRESSURE_SENSOR_BAR_PER_VOLT 2.0F
#define PIN_PRESSURE_SENSOR A7
#define PIN_COMPRESSOR 14
#define PIN_FILLING_VALVE1 15
#define PIN_FILLING_VALVE2 16

// L298 PWM outputs
// =============================================
#define PIN_PLATE_MOTOR  38
#define PIN_ROLLER_MOTOR 37
#define PIN_MIXER        36
#define PIN_OUT4         35

// Relays board
// =============================================
#define PIN_BATTER_VALVE   31
#define PIN_OILER_SOLENOID 32
#define PIN_RELAY3         33
#define PIN_RELAY4         34

// coin selector
#define PIN_COIN_SIGNAL    0

// LED strip
#define NUM_PIXELS 25
#define PIN_LEDS_DATA      28
#define PIN_LEDS_CLOCK     29
#define LEDS_INTERVAL      50

// buttons
#define PIN_BUTTON_MAINT   30


#endif /* CONFIG_H_ */
