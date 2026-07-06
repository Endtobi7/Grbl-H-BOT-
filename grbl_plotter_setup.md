# GRBL Plotter setup (GRBL 0.9i, CoreXY pen plotter)

## 1. Connection
1. Open GRBL Plotter.
2. Select the correct COM port for the Arduino Mega 2560.
3. Set baud rate to 115200.
4. Click Connect and confirm you see the Grbl 0.9i welcome message.

## 2. Initial settings upload
1. Open the console or command line in GRBL Plotter.
2. Send the settings from settings.txt (only the lines that start with "$"), then run $$ to verify.

## 3. Homing and alarms
1. On power-up, Grbl will be in ALARM. Click the Homing button or send $H.
2. After homing, the machine origin is (0,0,0).

## 4. Pen control with Z moves
1. Use Z moves for pen control (no M3/M5).
2. Example:
   - Pen up: G1 Z5 F300
   - Pen down: G1 Z0 F300
3. In GRBL Plotter, set the pen up/down Z values in the pen settings to match your machine.

## 5. Sender options
- Firmware type: GRBL 0.9 (if selectable).
- Status reports: leave default; real-time feed override and jogging should work out of the box.

## 6. First motion test
1. Jog X and Y a few mm to confirm CoreXY directions.
2. If an axis moves the wrong direction, change the $3 direction invert mask and test again.
3. Test Z up/down with short jogs before running a full plot.
