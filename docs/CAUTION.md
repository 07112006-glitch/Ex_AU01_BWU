# Caution

Read this before building or powering the hardware.

## 1. Water-level sensor is not used

A water-level sensor is wired to A2, and the diagram includes it, but `TANK_LEVEL_PIN` is marked **reserved** and is never read. The sketch relies solely on the tank switch on A1 to decide if water is available. If you expect the A2 sensor to protect against an empty tank, it will not.

## 2. Tank switch simulates both tank checks

The single SPST switch on A1 is used as both tank checks (`switchHasWater && simulatedFloatSwitchHasWater`). They are the same signal, so a faulty or stuck switch gives a false "water available" reading on both checks simultaneously.

## 3. Relay is active-low

`RELAY_ON = LOW`. Wiring the relay backwards (expecting HIGH to energize) will invert pump behavior: the pump would run when the sketch thinks it is off and vice versa.

## 4. Pump runtime is capped, not soil-driven shutoff

The pump runs for up to 5 seconds (`MAX_WATER_MS`) regardless of whether the soil has become moist. There is no mid-cycle soil re-check to stop early. Overwatering is possible if 5 seconds is too much for your soil/pot.

## 5. 30-second rewatering lockout

After a pump cycle completes, the system waits 30 seconds before watering again (`REWATER_DELAY_MS`). If the soil is still dry after 5 seconds, you must wait the full lockout before the next attempt.

## 6. Two separate 5V supplies

The diagram shows two 5V power-supply simulation models — one for the Arduino and one for the motor/relay load. Do not power the pump motor directly from the Arduino 5V pin; use a separate supply with a common ground.

## 7. Not tested on hardware

This documentation was written by inspecting the wiring and saved sketch. No simulation or physical hardware test was performed. Verify pin mappings, threshold values, and relay logic against your actual hardware before deploying.

## 8. Blocking tone is avoided, but Serial prints every second

The melody is non-blocking, but `loop()` prints to Serial every 1 second. This is harmless but adds minor overhead.

## 9. Soil threshold may need calibration

`SOIL_DRY_THRESHOLD = 600` is a fixed value. Depending on your soil sensor, soil type, and wiring, this threshold may read "dry" when wet or "wet" when dry. Test with your actual sensor before relying on it.
