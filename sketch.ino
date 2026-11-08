
// Automatic watering controller
// Manual switch A1 simulates BOTH tank checks.
// Switch closed = water available.
// Switch open = tank empty.
// Pump operates only when both simulated checks report water.

const uint8_t SOIL_PIN = A0;
const uint8_t TANK_SWITCH_PIN = A1;
const uint8_t TANK_LEVEL_PIN = A2; // Reserved; not used in simulation mode
const uint8_t RELAY_PIN = 8;
const uint8_t BUZZER_PIN = 9;
const uint8_t GREEN_LED_PIN = 10;
const uint8_t RED_LED_PIN = 11;

const int SOIL_DRY_THRESHOLD = 600;

const unsigned long SWITCH_DEBOUNCE_MS = 30;
const unsigned long SOIL_SAMPLE_MS = 1000;
const unsigned long MAX_WATER_MS = 5000;
const unsigned long REWATER_DELAY_MS = 30000;

// Active-low relay
const uint8_t RELAY_ON = LOW;
const uint8_t RELAY_OFF = HIGH;

// Alarm melody
const int MELODY_NOTES[] = {
  659,622,659,622,659,494,587,523,440,0,
  262,330,440,494,0,
  330,415,494,523,0
};

const unsigned int MELODY_MS[] = {
  200,200,200,200,200,200,200,200,600,300,
  200,200,200,600,300,
  200,200,200,600,300
};

const uint8_t MELODY_LEN =
  sizeof(MELODY_NOTES) / sizeof(MELODY_NOTES[0]);

uint8_t melodyIndex = 0;
unsigned long melodyNoteStart = 0;

void playCurrentNote() {
  if (MELODY_NOTES[melodyIndex] == 0) {
    noTone(BUZZER_PIN);
  } else {
    tone(BUZZER_PIN, MELODY_NOTES[melodyIndex]);
  }
}

void startMelody(unsigned long now) {
  melodyIndex = 0;
  melodyNoteStart = now;
  playCurrentNote();
}

void updateMelody(unsigned long now) {
  if (now - melodyNoteStart >= MELODY_MS[melodyIndex]) {
    melodyIndex = (melodyIndex + 1) % MELODY_LEN;
    melodyNoteStart = now;
    playCurrentNote();
  }
}

// System states
bool switchCandidate = false;
bool switchHasWater = false;
bool soilIsDry = false;
bool pumpRunning = false;
bool hasWatered = false;
bool alarmActive = false;

unsigned long switchCandidateSince = 0;
unsigned long lastSoilSample = 0;
unsigned long pumpStartedAt = 0;
unsigned long lastWateredAt = 0;

void pumpOff() {
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pumpRunning = false;
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pumpOff();

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  pinMode(TANK_SWITCH_PIN, INPUT_PULLUP);

  switchCandidate = (digitalRead(TANK_SWITCH_PIN) == LOW);
  switchCandidateSince = millis();

  Serial.begin(9600);
}

void loop() {
  const unsigned long now = millis();

  // Debounce manual tank switch.
  // LOW = water available.
  // HIGH = tank empty.

  const bool rawSwitchHasWater =
    (digitalRead(TANK_SWITCH_PIN) == LOW);

  if (rawSwitchHasWater != switchCandidate) {
    switchCandidate = rawSwitchHasWater;
    switchCandidateSince = now;
  }

  if (now - switchCandidateSince >= SWITCH_DEBOUNCE_MS) {
    switchHasWater = switchCandidate;
  }

  // Soil moisture sampling.
  if (now - lastSoilSample >= SOIL_SAMPLE_MS) {
    lastSoilSample = now;

    soilIsDry =
      (analogRead(SOIL_PIN) >= SOIL_DRY_THRESHOLD);

    Serial.print("Soil dry: ");
    Serial.print(soilIsDry ? "yes" : "no");

    Serial.print("  Switch wet: ");
    Serial.print(switchHasWater ? "yes" : "no");

    Serial.print("  Simulated float wet: ");
    Serial.println(switchHasWater ? "yes" : "no");
  }

  // Manual switch simulates both tank checks.
  const bool simulatedFloatSwitchHasWater = switchHasWater;

  const bool tankChecksAgreeWet =
    switchHasWater && simulatedFloatSwitchHasWater;

  // Tank empty: stop pump and activate alarm.
  if (!tankChecksAgreeWet) {
    pumpOff();
    hasWatered = false;

    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);

    if (!alarmActive) {
      startMelody(now);
      alarmActive = true;
    }

    updateMelody(now);
    return;
  }

  // Tank has water: stop alarm.
  if (alarmActive) {
    noTone(BUZZER_PIN);
    alarmActive = false;
  }

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);

  // Soil is wet: no watering needed.
  if (!soilIsDry) {
    pumpOff();
    hasWatered = false;
    return;
  }

  // Bounded watering cycle.
  if (pumpRunning) {
    if (now - pumpStartedAt >= MAX_WATER_MS) {
      pumpOff();
      lastWateredAt = now;
      hasWatered = true;
    }
  }
  else if (!hasWatered ||
           now - lastWateredAt >= REWATER_DELAY_MS) {

    digitalWrite(RELAY_PIN, RELAY_ON);
    pumpRunning = true;
    pumpStartedAt = now;
  }
}
