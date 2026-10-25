# STM32 Sensor Evaluation

A university project focused on evaluating different motion sensors using an STM32 microcontroller.

I worked with three sensors (BMI270, LSM6DSOX and MIS2DH) and compared their sampling rates, power consumption and behavior at different I²C speeds.

I also implemented a complementary filter in C to estimate angles using accelerometer and gyroscope data.

## What I worked on

- Sensor configuration and communication over I²C
- Reading sensor data using FIFO buffers
- Measuring sampling rates and current consumption
- Comparing sensor performance at different I²C speeds
- Implementing a complementary filter in C
- Debugging with an oscilloscope and other measurement tools

## Hardware

- STM32U5 Nucleo board
- BMI270, LSM6DSOX and MIS2DH sensors
- Oscilloscope and multimeter

## Code

The project contains several experiments and sensor configurations.

The FIFO and complementary filter implementation can be found in the `FIFO_AND_COMPLEMENTARY_FILTER` branch, mainly under `MKI197V1/`.

## About

Developed in 2025 as part of my Embedded Systems Engineering studies at the University of Freiburg.
