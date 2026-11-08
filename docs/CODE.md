# Code Walkthrough

File: `sketch.ino` (197 lines)

## Includes / constants (lines 1–41)

Pin definitions and thresholds are declared as `const` at the top:

```cpp
const uint8_t SOIL_PIN = A0;
const uint8_t TANK_SWITCH_PIN = A1;
const uint8_t TANK_LEVEL_PIN = A2; // Reserved; not used
const uint8_t RELAY_PIN = 8;
const uint8_t BUZZER_PIN = 9;
const uint8_t GREEN_LED_PIN = 10;
const uint8_t RED_LED_PIN = 11;

const int SOIL_DRY_THRESHOLD = 600;
```

Timing constants:

```cpp
const unsigned long SWITCH_DEBOUNCE_MS = 30;
const unsigned long SOIL_SAMPLE_MS = 1000;
const unsigned long MAX_WATER_MS = 5000;
const unsigned long REWATER_DELAY_MS = 30000;
```

Relay is active-low:

```cpp
const uint8_t RELAY_ON = LOW;
const uint8_t RELAY_OFF = HIGH;
```

Alarm melody data: `MELODY_NOTES[]` (frequencies) and `MELODY_MS[]` (durations), 19 notes total.

## Melody functions (lines 46–66)

- `playCurrentNote()` — Plays the current melody note on the buzzer, or silences it if frequency is 0.
- `startMelody(now)` — Resets melody index and starts from the first note.
- `updateMelody(now)` — Advances to the next note when the current note's duration has elapsed. Non-blocking.

## State variables (lines 68–80)

Booleans for switch, soil, pump, watered, and alarm states, plus `unsigned long` timers for debounce, sampling, pump start, and last watering.

## pumpOff() (lines 81–84)

Sets relay to `RELAY_OFF` (HIGH) and clears `pumpRunning`.

## setup() (lines 86–105)

- Configures relay, buzzer, and both LEDs as outputs.
- Starts with pump off, buzzer silent, both LEDs off.
- Configures tank switch pin as `INPUT_PULLUP`.
- Reads initial switch state and starts the debounce timer.
- Begins Serial at 9600 baud.

## loop() (lines 107–197)

Each iteration:

1. Reads `millis()` as `now`.
2. **Debounce tank switch** (lines 110–124): Reads A1. `LOW` = water available. If raw differs from candidate, update candidate and reset timer. After 30 ms stability, commit to `switchHasWater`.
3. **Sample soil** (lines 127–141): Every 1 second, read A0. `soilIsDry = reading >= 600`. Print soil/switch status to Serial.
4. **Tank agreement** (lines 144–147): Both the physical switch and its simulated copy must agree water is present.
5. **Tank empty branch** (lines 150–164): Pump off, red LED on, green LED off, start/update alarm melody, return.
6. **Tank wet branch** (lines 167–173): Stop alarm if active. Green LED on, red LED off.
7. **Soil wet branch** (lines 176–180): Pump off, return.
8. **Bounded watering** (lines 183–196): If pump running and max time elapsed, stop pump and record watering. Otherwise, if eligible (not watered or rewater delay passed), turn pump on.

## Key behaviors

- Non-blocking: no `delay()` calls anywhere.
- Pump auto-shuts off after 5 seconds even if soil is still dry.
- Rewatering blocked for 30 seconds after a cycle ends.
- Alarm only active when tank is empty.
- Tank switch simulates both tank checks; separate water-level sensor on A2 is unused.
