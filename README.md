# HVAC Control System

This repository contains a C program for a Heating, Ventilation, and Air Conditioning (HVAC) control system. The program simulates the control of a climate control system to maintain a desired temperature within a certain range.

## Table of Contents

- [Overview](#overview)
- [Default and Range Values](#default-and-range-values)
- [System Components](#system-components)
- [Initialization](#initialization)
- [Control Logic](#control-logic)
- [Simulation](#simulation)
- [Saving Data](#saving-data)
- [Usage](#usage)
- [Recommendations](#recommendations)

## Overview

The HVAC control system is designed to maintain a comfortable temperature in a controlled environment. It includes various components such as a compressor, condenser fan, water pump, electric resistance, and evaporator fan. The control system adjusts these components based on the difference between the desired temperature and the actual temperature.

## Default and Range Values

The program uses default values and ranges for various parameters to control the HVAC system. These values can be adjusted to suit different scenarios.

- Default Pressure: 5 bars
- Default Temperature: 22°C
- Temperature Range: 18°C to 30°C
- Pressure Range: 0.5 bars to 25 bars
- ...

## System Components

The HVAC system components include:

- Desired Temperature
- Actual Temperature
- High and Low Pressure
- Compressor State (OFF, MIN, MED, MAX)
- Condenser Fan Speed
- Water Pump Speed
- Resistance State (OFF, MIN, MED, MAX)
- Evaporator Fan Speed
- Zone Temperature

## Initialization

The `init_system` function initializes the system with default values. You can modify these default values to suit your requirements.

## Control Logic

The `control_system` function implements the control logic for the HVAC system. It adjusts system components based on the desired temperature and the current temperature. The control logic includes cooling and heating modes, compressor control, fan speed adjustment, and more.

## Simulation

The program simulates the HVAC system's behavior, including random fluctuations in temperature and adjustments to system components. You can choose to use random values for temperature fluctuations, specify specific values or use a PID controller to adjust the evaporating fan speed according to the difference between the desired temperature and the actual temperature uncommenting any of the following lines of code:
- `//#define USE_RANDOM_VALUES`
- `//#define USE_SPECIFIC_VALUES`
- `//#define USE_PID`

## Saving Data

The program can save system data to a CSV file, including the desired temperature, actual temperature, pressure, compressor state, fan speeds, and resistance state.

## Usage

To use the program, you can either simulate the system with random or specific values or exit the simulation. You can enter the desired temperature within the specified range (18°C to 30°C) to control the HVAC system's behavior.

## Recommendations

The use of a C compiler, such as GCC (GNU Compiler Collection) and the GDB debugger (GNU Debugger) is recommended. In addition, an IDE such as Visual Studio Code or Eclipse can be used to further simplify the debugging process. It is also possible to use an online C debugger.
The following libraries are required: stdio.h, stdint.h, stdlib.h, stdbool.h, time.h, unistd.h.

---

**Note:** This README provides an overview of the HVAC control system program. Detailed explanations of the code can be found in the source code comments.