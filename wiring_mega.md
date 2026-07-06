# Arduino Mega 2560 wiring (CoreXY + Z rack and pinion)

This wiring map matches the CPU map in cpu_map.h. Use a common ground between the Mega and all TB6560 drivers.

## Stepper drivers (TB6560)

| Axis | Signal | Mega pin | AVR port/bit | TB6560 terminal |
| --- | --- | --- | --- | --- |
| X (CoreXY A motor) | STEP | A0 | PF0 | PUL- |
| X (CoreXY A motor) | DIR  | 49 | PL0 | DIR- |
| Y (CoreXY B motor) | STEP | A1 | PF1 | PUL- |
| Y (CoreXY B motor) | DIR  | 48 | PL1 | DIR- |
| Z (Rack & pinion) | STEP | A2 | PF2 | PUL- |
| Z (Rack & pinion) | DIR  | 47 | PL2 | DIR- |

TB6560 input side power:
- Connect each driver PUL+, DIR+, EN+ to +5V from the Mega.
- Connect all TB6560 signal grounds to Mega GND.
- If your TB6560 board uses opto inputs that require active-low, this matches the default $4=0 setting.

Note: The firmware disables the stepper enable pin. Leave EN- unconnected or tie it to GND per your driver board guidance.

## Limit switches (normally open to GND)

| Axis | Mega pin | AVR port/bit | Wiring |
| --- | --- | --- | --- |
| X limit | 52 | PB1 | Switch between pin and GND |
| Y limit | 51 | PB2 | Switch between pin and GND |
| Z limit | 50 | PB3 | Switch between pin and GND |

Notes:
- Internal pullups are enabled by default, so no external resistors are required.
- Keep limit switch wires away from motor power wiring when possible.

## Power and ground

- Share GND between Mega 2560 and all TB6560 driver signal grounds.
- Provide separate motor power to the TB6560 boards per their datasheet.
