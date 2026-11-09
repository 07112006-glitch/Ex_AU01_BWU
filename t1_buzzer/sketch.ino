// Tank alarm test: buzzer on D9, A1 wired to GND with a jumper.
// A1 connected to GND (LOW)  = water available -> buzzer silent
// A1 disconnected (HIGH)     = tank empty      -> alarm melody plays

const uint8_t TANK_PIN   = A1;
const uint8_t BUZZER_PIN = 9;

const unsigned long DEBOUNCE_MS = 30;

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

bool tankHasWater = false;
bool candidate = false;
bool alarmActive = false;
unsigned long candidateSince = 0;

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

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  pinMode(TANK_PIN, INPUT_PULLUP);

  candidate = (digitalRead(TANK_PIN) == LOW);
  tankHasWater = candidate;
  candidateSince = millis();

  Serial.begin(9600);
  Serial.println("Tank alarm test started");
}

void loop() {
  const unsigned long now = millis();

  // Debounce A1: LOW = water available, HIGH = tank empty.
  const bool raw = (digitalRead(TANK_PIN) == LOW);

  if (raw != candidate) {
    candidate = raw;
    candidateSince = now;
  }

  if (now - candidateSince >= DEBOUNCE_MS && tankHasWater != candidate) {
    tankHasWater = candidate;
    Serial.println(tankHasWater ? "Tank: water available" : "Tank: EMPTY");
  }

  if (!tankHasWater) {
    if (!alarmActive) {
      startMelody(now);
      alarmActive = true;
    }
    updateMelody(now);
  } else if (alarmActive) {
    noTone(BUZZER_PIN);
    alarmActive = false;
  }
}