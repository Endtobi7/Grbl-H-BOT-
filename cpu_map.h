/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The cpu_map.h files serve as a central pin mapping selection file for different processor
   types, i.e. AVR 328p or AVR Mega 2560. Each processor has its own pin mapping file.
   (i.e. cpu_map_atmega328p.h)  Grbl officially supports the Arduino Uno, but the 
   other supplied pin mappings are supplied by users, so your results may vary. */

// NOTE: With new processors, only add the define name and filename to use.

#ifndef cpu_map_h
#define cpu_map_h


#ifdef CPU_MAP_ATMEGA328P // (Arduino Uno) Officially supported by Grbl.
  #include "cpu_map/cpu_map_atmega328p.h"
#endif // CPU_MAP_ATMEGA328P

#ifdef CPU_MAP_ATMEGA2560 // (Arduino Mega 2560) Custom map
  #ifdef GRBL_PLATFORM // Mega2560
    #error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM // Mega2560
  #endif // Mega2560

  #define GRBL_PLATFORM "Atmega2560" // Mega2560

  // Serial port pins. // Mega2560
  #define SERIAL_RX USART0_RX_vect // Mega2560
  #define SERIAL_UDRE USART0_UDRE_vect // Mega2560

  // Stepper step pins. NOTE: All step bits must be on the same port. // Mega2560
  #define STEP_DDR DDRF // Mega2560
  #define STEP_PORT PORTF // Mega2560
  #define X_STEP_BIT 0 // Mega pin A0 = PF0
  #define Y_STEP_BIT 1 // Mega pin A1 = PF1
  #define Z_STEP_BIT 2 // Mega pin A2 = PF2
  #define STEP_MASK ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // Mega2560

  // Stepper direction pins. NOTE: All direction bits must be on the same port. // Mega2560
  #define DIRECTION_DDR DDRL // Mega2560
  #define DIRECTION_PORT PORTL // Mega2560
  #define X_DIRECTION_BIT 0 // Mega pin 49 = PL0
  #define Y_DIRECTION_BIT 1 // Mega pin 48 = PL1
  #define Z_DIRECTION_BIT 2 // Mega pin 47 = PL2
  #define DIRECTION_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // Mega2560

  // Stepper enable pin (active low). // Mega2560
  #define STEPPERS_DISABLE_DDR DDRD // Mega2560
  #define STEPPERS_DISABLE_PORT PORTD // Mega2560
  #define STEPPERS_DISABLE_BIT 7 // Mega pin 38 = PD7
  #define STEPPERS_DISABLE_MASK (1<<STEPPERS_DISABLE_BIT) // Mega2560

  // Limit switch input pins and pin-change interrupt. // Mega2560
  #define LIMIT_DDR DDRB // Mega2560
  #define LIMIT_PORT PORTB // Mega2560
  #define LIMIT_PIN PINB // Mega2560
  #define X_LIMIT_BIT 1 // Mega pin 52 = PB1 (PCINT1)
  #define Y_LIMIT_BIT 2 // Mega pin 51 = PB2 (PCINT2)
  #define Z_LIMIT_BIT 3 // Mega pin 50 = PB3 (PCINT3)
  #define LIMIT_INT PCIE0 // Mega2560
  #define LIMIT_INT_vect PCINT0_vect // Mega2560
  #define LIMIT_PCMSK PCMSK0 // Mega2560
  #define LIMIT_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // Mega2560

  // Spindle enable and direction pins. // Mega2560
  #define SPINDLE_ENABLE_DDR DDRH // Mega2560
  #define SPINDLE_ENABLE_PORT PORTH // Mega2560
  #define SPINDLE_ENABLE_BIT 3 // Mega pin 6 = PH3
  #define SPINDLE_DIRECTION_DDR DDRE // Mega2560
  #define SPINDLE_DIRECTION_PORT PORTE // Mega2560
  #define SPINDLE_DIRECTION_BIT 3 // Mega pin 5 = PE3

  // Coolant output pins. // Mega2560
  #define COOLANT_FLOOD_DDR DDRH // Mega2560
  #define COOLANT_FLOOD_PORT PORTH // Mega2560
  #define COOLANT_FLOOD_BIT 5 // Mega pin 8 = PH5
  #ifdef ENABLE_M7 // Mega2560
    #define COOLANT_MIST_DDR DDRH // Mega2560
    #define COOLANT_MIST_PORT PORTH // Mega2560
    #define COOLANT_MIST_BIT 6 // Mega pin 9 = PH6
  #endif // Mega2560

  // Control input pins. NOTE: All control pins must be on the same port. // Mega2560
  #define CONTROL_DDR DDRK // Mega2560
  #define CONTROL_PIN PINK // Mega2560
  #define CONTROL_PORT PORTK // Mega2560
  #define RESET_BIT 0 // Mega analog pin 8 = PK0
  #define FEED_HOLD_BIT 1 // Mega analog pin 9 = PK1
  #define CYCLE_START_BIT 2 // Mega analog pin 10 = PK2
  #define SAFETY_DOOR_BIT 3 // Mega analog pin 11 = PK3
  #define CONTROL_INT PCIE2 // Mega2560
  #define CONTROL_INT_vect PCINT2_vect // Mega2560
  #define CONTROL_PCMSK PCMSK2 // Mega2560
  #define CONTROL_MASK ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT)) // Mega2560

  // Probe input pin. // Mega2560
  #define PROBE_DDR DDRK // Mega2560
  #define PROBE_PIN PINK // Mega2560
  #define PROBE_PORT PORTK // Mega2560
  #define PROBE_BIT 7 // Mega analog pin 15 = PK7
  #define PROBE_MASK (1<<PROBE_BIT) // Mega2560

  // Start of PWM spindle definitions (unused when VARIABLE_SPINDLE is disabled). // Mega2560
  #ifdef VARIABLE_SPINDLE // Mega2560
    #define PWM_MAX_VALUE 65535.0 // Mega2560
    #define TCCRA_REGISTER TCCR4A // Mega2560
    #define TCCRB_REGISTER TCCR4B // Mega2560
    #define OCR_REGISTER OCR4B // Mega2560
    #define COMB_BIT COM4B1 // Mega2560
    #define WAVE0_REGISTER WGM40 // Mega2560
    #define WAVE1_REGISTER WGM41 // Mega2560
    #define WAVE2_REGISTER WGM42 // Mega2560
    #define WAVE3_REGISTER WGM43 // Mega2560
    #define SPINDLE_PWM_DDR DDRH // Mega2560
    #define SPINDLE_PWM_PORT PORTH // Mega2560
    #define SPINDLE_PWM_BIT 4 // Mega pin 7 = PH4
  #endif // Mega2560
#endif // CPU_MAP_ATMEGA2560

/* 
#ifdef CPU_MAP_CUSTOM_PROC
  // For a custom pin map or different processor, copy and edit one of the available cpu
  // map files and modify it to your needs. Make sure the defined name is also changed in
  // the config.h file.
#endif
*/

#endif
