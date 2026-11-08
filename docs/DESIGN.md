# System Design

## Architecture

The controller runs a single non-blocking `loop()` on the Arduino UNO. All timing is handled with `millis()` so the sketch never blocks on `delay()`.

## State variables

| Variable | Type | Meaning |
|----------|------|---------|
| `switchCandidate` | bool | Raw debounced candidate for tank switch |
| `switchHasWater` | bool | Confirmed tank switch state after debounce |
| `soilIsDry` | bool | True when soil reading >= threshold |
| `pumpRunning` | bool | True while relay is energized |
| `hasWatered` | bool | True after a completed watering cycle |
| `alarmActive` | bool | True while the empty-tank melody plays |

## Timing constants

| Constant | Value | Purpose |
|----------|-------|---------|
| `SWITCH_DEBOUNCE_MS` | 30 | Tank switch debounce window |
| `SOIL_SAMPLE_MS` | 1000 | Soil sampling period |
| `MAX_WATER_MS` | 5000 | Maximum pump run time |
| `REWATER_DELAY_MS` | 30000 | Delay before next watering |

## Flow

1. **Debounce tank switch** — Read A1. If the raw reading differs from the candidate, reset the debounce timer. After 30 ms of stability, commit to `switchHasWater`.
2. **Sample soil** — Every 1 second, read A0. Dry if >= 600. Print status to Serial.
3. **Tank check** — Both the physical switch and its simulated duplicate must report water. The switch simulates both checks.
4. **Tank empty** — Turn pump off, red LED on, green LED off, start/continue alarm melody, return early.
5. **Tank has water** — Stop alarm, green LED on, red LED off.
6. **Soil wet** — Turn pump off, return.
7. **Soil dry** — If pump running and `MAX_WATER_MS` elapsed, turn pump off and record `lastWateredAt`. Otherwise, if not watered or rewater delay elapsed, turn pump on.

## Relay logic

The relay is active-low:
- `RELAY_ON = LOW` — pump energized.
- `RELAY_OFF = HIGH` — pump off.

## Alarm melody

A 19-note melody stored in `MELODY_NOTES[]` with per-note durations in `MELODY_MS[]`. Notes play sequentially, non-blocking, via `updateMelody()`. A frequency of 0 means silence (no tone).

## Pin assignments

| Pin | Function | Direction |
|-----|----------|-----------|
| A0 | Soil moisture | INPUT |
| A1 | Tank switch | INPUT_PULLUP |
| A2 | Water-level sensor | Reserved |
| 8 | Relay | OUTPUT |
| 9 | Buzzer | OUTPUT |
| 10 | Green LED | OUTPUT |
| 11 | Red LED | OUTPUT |

## Diagram

![Breadboard layout](assets/png/auto_watering_breadboard_realistic.png)
