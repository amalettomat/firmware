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
#define ROLLER_SPEED 70

// rozel
// =============================================
#define ROZEL_SPEED_UP 150
#define ROZEL_SPEED_DOWN 100
#define PIN_ROZEL_ENDSTOP_DOWN 1
#define PIN_ROZEL_ENDSTOP_UP 2

// display
// =============================================
#define DISPLAY_INTERVAL 200
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
#define PIN_PLATE_MOTOR  38 // OUT1
#define PIN_ROLLER_MOTOR 37 // OUT2
#define PIN_ROZEL_UP     36 // OUT3
#define PIN_ROZEL_DOWN   35 // OUT4

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
