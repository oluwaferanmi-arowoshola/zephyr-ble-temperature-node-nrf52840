# Zephyr RTOS BLE Temperature Monitoring Node on nRF52840 DK

## Overview

This project implements and evaluates a Zephyr RTOS-based BLE temperature monitoring node on the Nordic nRF52840 Development Kit. The system uses a simulated temperature source running in a Zephyr sensor thread, protects shared application state with a mutex, exposes the latest temperature through a custom BLE GATT service, and validates BLE reads from a mobile phone using nRF Connect.

The project also evaluates Zephyr RTOS scheduler behavior through multithreading, priority, starvation, mutex, and timing experiments. Final timing measurements compare baseline sensor-thread execution against execution under higher-priority CPU interference.

This project was completed as part of EE 695 research work in Electrical and Computer Engineering.

---

## Hardware and Software Stack

**Hardware**

- Nordic nRF52840 Development Kit
- Bluetooth Low Energy capable mobile phone
- USB serial connection for logging/debugging
- Nordic Power Profiler Kit II used for exploratory setup testing

**Software**

- Zephyr RTOS
- Embedded C
- West build system
- CMake / Ninja
- Nordic nRF Connect mobile app
- MATLAB for result plotting and timing visualization

---

## Project Features

- Zephyr RTOS application development on the nRF52840 DK
- BLE peripheral configuration and advertising
- Custom BLE GATT service
- Readable BLE temperature characteristic
- Simulated periodic temperature updates
- Mutex-protected shared temperature state
- RTOS thread creation and scheduling experiments
- Priority scheduling and starvation experiments
- Mutex-protected UART output
- Application-level latency measurement using `k_uptime_get()`
- Baseline and interference timing comparison
- MATLAB-generated performance plots

---

## System Architecture

The project is organized around a simulated embedded sensing node.

1. A Zephyr sensor thread periodically updates a simulated temperature value.
2. The latest temperature value is stored in shared application state.
3. A mutex protects access to the shared temperature value.
4. A custom BLE GATT characteristic exposes the latest value.
5. A mobile phone connects using nRF Connect and reads the characteristic.
6. Timing experiments measure BLE-read latency and sensor-thread scheduling behavior.

![System Architecture](images/system-architecture.png)

---

## BLE Temperature Node

The final BLE prototype uses a custom service and characteristic to expose the latest simulated temperature reading.

Core behavior:

- Initializes the Zephyr Bluetooth stack
- Starts BLE advertising
- Registers a custom GATT service
- Runs a periodic simulated sensor thread
- Updates the temperature value every cycle
- Allows a mobile client to read the latest value

The final BLE node source code is located here:

```text
src/phase3_ble_temperature_node/main.c
```
nRF Connect validation:

---

## RTOS Scheduler Experiments

The project includes a set of Zephyr RTOS experiments used to study thread behavior.

Experiment files are located in:

```text
src/phase2_scheduler_experiments/
```

Included experiments:

- Basic multithreading
- Priority scheduling
- UART output interference
- Mutex-protected UART output
- High-priority busy thread behavior
- Starvation behavior
- Busy thread with short sleep
- Equal-priority thread behavior

These experiments helped validate how Zephyr schedules threads under different priority and blocking conditions.

---

## Timing and Performance Analysis

Phase 4 measured application-level timing behavior using Zephyr uptime timestamps.

Timing-analysis code is located in:

```text
src/phase4_timing_analysis/
```

Included timing experiments:

- BLE read latency measurement
- Baseline sensor-thread timing
- Sensor-thread timing under higher-priority CPU interference
- LED test for exploratory power-measurement setup

### BLE Read Latency

The BLE latency experiment measured the time between the most recent simulated sensor update and the BLE characteristic read.

### Sensor Thread Timing

The baseline timing experiment measured how close the sensor thread stayed to its intended period under normal conditions.

The interference experiment introduced a higher-priority busy thread to observe how CPU load affected sensor-thread timing.

---

## Key Results

- Implemented a working Zephyr RTOS BLE peripheral on the Nordic nRF52840 DK.
- Validated custom BLE service discovery and characteristic reads using nRF Connect.
- Demonstrated mutex-protected shared state between a simulated sensor thread and BLE read callback.
- Compared baseline sensor-thread timing against timing under higher-priority CPU interference.
- Used MATLAB to generate performance plots for BLE-read latency and scheduler timing behavior.
- Attempted exploratory power-measurement setup using Nordic Power Profiler Kit II, but did not treat the readings as formal quantitative results due to measurement setup limitations.

---

## Repository Structure

```text
│
├── docs/
├── src/
├── analysis/
├── results/
├── images/
├── README.md
└── .gitignore
```

---

## How to Build and Run

This project was developed using Zephyr RTOS and the Nordic nRF52840 DK.

A typical Zephyr build command for the nRF52840 DK is:

```text
west build -b nrf52840dk/nrf52840 path/to/application --pristine
```

To flash:

```text
west flash
```

To monitor serial output:

```text
python -m serial.tools.miniterm COM_PORT 115200
```

Replace COM_PORT with the actual serial port assigned to the nRF52840 DK.

---

## Notes and Limitations

- The temperature value is simulated; this version does not use an external physical temperature sensor.
- BLE validation was performed using the nRF Connect mobile app.
- Power measurement setup was explored using Nordic PPK2, but the quantitative readings were not treated as final due to setup limitations.
- The code files are organized by experiment phase rather than as a single production firmware application.

---

## Skills Demonstrated

- Embedded C development
- Zephyr RTOS application development
- BLE GATT service implementation
- Nordic nRF52840 DK bring-up
- RTOS threading and synchronization
- Mutex-protected shared data
- Scheduler behavior analysis
- Timing measurement using k_uptime_get()
- Serial debugging
- Mobile BLE validation with nRF Connect
- MATLAB-based result visualization
- Technical documentation and research reporting

---

## Author

Oluwaferanmi Arowoshola

M.S. Electrical & Computer Engineering

Embedded Systems · Real-Time Systems · IoT · Semiconductor Fabrication
