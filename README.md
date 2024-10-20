# EV Charging Station
## University of Bucharest, Faculty of Mathematics and Computer Science, 2024

This project is an Arduino-based (ATMega328P) controller for a "charging station", featuring LED indicators and buttons to control the charging process.

## Table of Contents

1. [Overview](#overview)
2. [Hardware Setup](#hardware-setup)
3. [Installation](#installation)
4. [Usage](#usage)

## Overview

The Charging Station uses two buttons to start and stop the charging process, while the blue LEDs and the RGB LED provides visual feedback. BTN1 initiates charging and BTN2 stops the process if long-pressed for at least 3 seconds. [See schematics folder for more info](./assets/schematics/).

## Hardware Setup

### Components
- Arduino UNO board
- x2 push buttons (BTN1, BTN2)
- x4 blue LEDs - for charge process simulation (LED1-4)
- x1 RGB LED - red/green color shows station availability (RLED, GLED)
- x6 220 resistors (for LEDs) - _optionally_ + 1 220 resistor for the blue pin in the RGB LED.
- x2 1K resistors (for buttons)
- Jumper wires

### Connections
- BTN1 (Pin 3)
- BTN2 (Pin2)
- LED1 to LED4 (Pins 7 to 10)
- GLED (Pin 5)
- RLED (Pin 6)
- _Optional_: BLED (Pin4) - in case you want to add/modify something

> [!NOTE]
Both buttons (BTN1 & BTN2) use **pull-down** resistors.

### Project Schematic

![Schematic image](./assets/schematics/scehamtic.png "Project Schematic")

> [!NOTE]
In the [Schematics folder](./assets/schematics/) you can find a .sim file that can be opened in [SimulIDE](https://simulide.com/p/).

## Installation
1. Clone the repository:
```bash
git clone https://github.com/Adrifot/EV-Charger-Arduino---ATMega-328P-.git
```

2. Open the `main.cpp` file. You can convert it to a `.ino` file to open it in Arduino IDE. Otherwise, you can use the [PlatformIO extension for VSCode](https://platformio.org/install/ide?install=vscode).

3. Connect your Arduino board and upload the code. Open the serial monitor.

## Usage
1. Wait for the green LED to turn on and the "charging station available" message to appear on the monitor.

2. Press **BTN1** to start charging. The green LED will become RED and the blue LEDs will start blinking. 

3. Wait for the process to finish or press and hold **BTN2** for 3 seconds to forcefully stop the charging process.