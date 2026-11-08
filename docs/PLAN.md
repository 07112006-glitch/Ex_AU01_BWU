# Project Plan

## Goal

Build an automatic plant-watering controller around an Arduino UNO that:
- Monitors soil moisture.
- Confirms water availability before pumping.
- Runs the pump for a bounded duration.
- Alerts when the tank is empty.

## Scope

### In scope
- Soil moisture sensing on A0.
- Tank switch input on A1 with debouncing.
- Relay-controlled pump on pin 8.
- Status LEDs (green system, red tank-empty).
- Piezo buzzer alarm melody.
- Bounded pump cycle (5 s max) with rewatering delay (30 s).
- Serial debug output at 9600 baud.

### Out of scope
- The water-level sensor on A2 (reserved, not read).
- WiFi / IoT connectivity despite the original description mentioning "IoT based."
- Automatic calibration of the soil moisture threshold.
- Multiple plant zones or multiple pumps.

## Components

| Component | Qty | Role |
|-----------|-----|------|
| Arduino UNO | 1 | Main controller |
| Soil moisture sensor | 1 | A0 input |
| SPST tank switch | 1 | A1 input (simulates tank level) |
| Water-level sensor | 1 | A2 (reserved, unused) |
| 1-channel 5V relay module | 1 | Pump switch (pin 8) |
| DC gear motor | 1 | Mini-pump load |
| Piezo buzzer | 1 | Alarm (pin 9) |
| Green LED + resistor | 1 | System status (pin 10) |
| Red LED + resistor | 1 | Tank-empty warning (pin 11) |
| 5V power supply (simulation) | 2 | Arduino and motor/relay load |

## References

- Circuit designed in Cirkit Designer: https://app.cirkitdesigner.com/project/10226882-e790-4eb3-85e8-ad76758a72bc

## Timeline notes

This documentation was written after inspecting the wiring and saved sketch. No physical hardware test or simulation run was performed.
