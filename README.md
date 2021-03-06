# A2Z-CAR (RobotCar)

## Description

Create a self-driving car, with a bunch of sensors and tune the car to get the best
performance. The main board will be an STM B-L475E-IOT01A, the secondary telemetry and logging board is the STM32F746G-DISCOVERY.

The main goals are to:
- follow a track (line)
- implement proper motor control
- get familiar with Matlab
- log data and do telemetry

## Main features
- servo steering control
- PI torque control of driver motor
- MATLAB motor model and controller parameters
- line following
- collision avoidance
- plotting data onto the LCD
- logging onto SD card
- USB mass storage device
- telemetry on WebServer
- 3D vision based control

## Technology
- CMSIS-OS (freeRTOS)
- STM32F7 HAL
- STM32L4 HAL
- STM32 GUI framework
- SD card
- lwIP
- Matlab
