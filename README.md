# 🔋 Smart Battery Management System (BMS) Simulator

A modular C-based Smart Battery Management System simulator designed to monitor battery parameters, simulate charging and discharging, calculate battery state information, and detect battery protection faults.

## 📌 Project Overview

This project simulates the core functionality of a Battery Management System (BMS) for a 4-cell battery pack.

The system monitors important battery parameters and performs protection checks to identify abnormal operating conditions.

### Monitored Parameters

- Individual cell voltage
- Cell temperature
- Pack current
- State of Charge (SOC)
- State of Health (SOH)
- Charging and discharging status
- Battery protection faults
- Fault logs

## 🚀 Features

- 4-cell battery pack simulation
- Cell voltage monitoring
- Temperature monitoring
- Current monitoring
- SOC calculation
- SOH calculation
- Charging simulation
- Discharging simulation
- Under-voltage protection
- Over-voltage protection
- Under-temperature / over-temperature monitoring
- Over-current protection
- Fault detection and logging
- Menu-driven interface
- Modular multi-file C implementation

## 🧩 BMS Protection

The simulator checks battery parameters against predefined safety limits.

### Voltage Protection
Detects:
- Under-voltage
- Over-voltage

### Temperature Protection
Detects:
- Low temperature
- High temperature

### Current Protection
Detects:
- Over-current conditions

Detected faults are recorded in the fault log for monitoring and debugging.

## 🛠️ Technologies Used

- C Programming
- Embedded C Concepts
- Structures
- Arrays
- Functions
- Pointers
- Bitwise Operations
- File Handling
- Modular Programming
- Debugging

## 📂 Project Structure

```text
smart-battery-management-system/
│
├── main.c
├── bms.c
├── bms.h
├── fault_log.c
└── README.md
