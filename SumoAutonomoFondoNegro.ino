// ------------------------------
// Pin Configuration
// ------------------------------
int motB0 = 2;
int motB1 = 3; // PWM
int motA1 = 4;
int motA0 = 5; // PWM

int sensorleft   = A7;
int sensorcenter = A6;
int sensorright  = A5;

int buzzer = 6; // Buzzer pin

int echo   = 8;
int triger = 7;

// ------------------------------
// Operation Constants
// ------------------------------
const int  LINE_THRESHOLD        = 550;  // White border detection threshold (2.5 cm)
const int  ATTACK_DISTANCE_CM    = 35;   // Close distance to attack at full speed (cm)
const int  TRACKING_DISTANCE_CM  = 77;   // Maximum distance to track opponent (cm) - Full ring size
const long MIN_VALID_DISTANCE_CM = 2;    // Minimum valid distance of ultrasonic sensor
const long MAX_VALID_DISTANCE_CM = 77;   // Maximum valid distance (ring size)
const long INVALID_DISTANCE_CM   = 1000; // Value used when measurement is unreliable
const unsigned long SEARCH_TIME = 150; // Time between search direction changes (ms) - Faster search

// ------------------------------
// State Variables
// ------------------------------
int l1 = 0; // Right sensor
int l2 = 0; // Center sensor
int l3 = 0; // Left sensor

unsigned long lastSearch = 0;
int searchDirection = 0; // 0=right, 1=left
long lastValidDistance = INVALID_DISTANCE_CM; // Track last valid distance reading
unsigned long lastOpponentTime = 0; // Time when opponent was last detected
bool opponentTracking = false; // Track if currently tracking opponent
unsigned long lastSoundTime = 0; // Last time sound was played


// ------------------------------
// Function Prototypes
// ------------------------------
void readLineSensors();
bool isLineDetected();
void avoidBorder();
void searchOpponent();
void trackOpponent(long distance);
void soundOpponent();
void soundBorder();
void soundSearch();
long distance();

void stop();
void backward();
void forward();
void forwardFast();
void forwardMedium();
void forwardSlow();
void left();
void right();
void leftWithForward();
void rightWithForward();
void backwardRight();
void backwardLeft();

// ------------------------------
// Setup
// ------------------------------
void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(motA0, OUTPUT);
  pinMode(motA1, OUTPUT);
  pinMode(motB0, OUTPUT);
  pinMode(motB1, OUTPUT);

  // Buzzer
  pinMode(buzzer, OUTPUT);

  // Ultrasonic
  pinMode(echo, INPUT);
  pinMode(triger, OUTPUT);
  digitalWrite(triger, LOW);

  // Line sensors
  pinMode(sensorright, INPUT);
  pinMode(sensorcenter, INPUT);
  pinMode(sensorleft, INPUT);

  // Motors initially off
  stop();
}

// ------------------------------
// Main Loop
// ------------------------------
void loop() {
  // Read line sensors
  readLineSensors();

  // Measure distance to front obstacle
  long d = distance();

  // PRIORITY 1: If white border detected, avoid border
  if (isLineDetected()) {
    lastValidDistance = INVALID_DISTANCE_CM; // Reset tracking
    opponentTracking = false; // Reset opponent tracking
    avoidBorder();
    return;
  }

  // PRIORITY 2: If opponent detected at any valid distance, track and attack
  if (d >= MIN_VALID_DISTANCE_CM && d <= TRACKING_DISTANCE_CM) {
    // Play sound when first detecting opponent (not continuously)
    if (!opponentTracking) {
      soundOpponent();
      lastSoundTime = millis();
    }
    opponentTracking = true;
    lastValidDistance = d;
    lastOpponentTime = millis();
    trackOpponent(d); // Track or attack based on distance
    return;
  }
  
  // Reset tracking flag when no opponent detected
  if (opponentTracking) {
    opponentTracking = false;
  }

  // PRIORITY 3: Continue tracking for a short time after losing signal (persistence)
  unsigned long timeSinceLastOpponent = millis() - lastOpponentTime;
  if (timeSinceLastOpponent < 200 && lastValidDistance < TRACKING_DISTANCE_CM) {
    // Continue advancing forward briefly after losing detection
    forwardMedium(); // Faster persistence
    return;
  }

  // PRIORITY 4: Actively search for opponent
  lastValidDistance = INVALID_DISTANCE_CM;
  searchOpponent();
}

// ------------------------------
// Line Sensor Reading
// ------------------------------
void readLineSensors() {
  // Order preserved: l3 left, l2 center, l1 right
  l3 = analogRead(sensorleft);
  l2 = analogRead(sensorcenter);
  l1 = analogRead(sensorright);
}

// Returns true if any sensor exceeds line threshold
bool isLineDetected() {
  return (l1 > LINE_THRESHOLD || l2 > LINE_THRESHOLD || l3 > LINE_THRESHOLD);
}

// ------------------------------
// Composite Maneuvers
// ------------------------------
// Optimized sequence to quickly avoid white border - NO BACKWARD MOVEMENT
void avoidBorder() {
  soundBorder();
  stop();
  delay(50);  // Brief stop before turning
  
  // Turn immediately while moving forward to quickly get away from border
  // NO BACKWARD MOVEMENT - only turning forward to avoid border
  if (l1 > LINE_THRESHOLD || l2 > LINE_THRESHOLD) {
    // Border detected more to the right, turn left while advancing
    leftWithForward();  // Turn left while moving forward
    delay(500);  // Turn duration to ensure we move away from border
  } else {
    // Border detected more to the left, turn right while advancing
    rightWithForward();  // Turn right while moving forward
    delay(500);  // Turn duration to ensure we move away from border
  }
  searchDirection = 0; // Reset search
}

// ------------------------------
// Track and Attack Opponent
// ------------------------------
void trackOpponent(long dist) {
  // Close range: Attack at full speed
  if (dist < ATTACK_DISTANCE_CM) {
    // Sound more frequently when very close
    unsigned long currentTime = millis();
    if (currentTime - lastSoundTime > 200) { // Sound every 200ms when close
      soundOpponent();
      lastSoundTime = currentTime;
    }
    forwardFast(); // Full speed attack
    return;
  }
  
  // Medium range: Advance quickly towards opponent
  // Always go straight when opponent is detected - no turning!
  forwardMedium(); // Faster than search speed
}

// ------------------------------
// Active Opponent Search
// ------------------------------
void searchOpponent() {
  unsigned long currentTime = millis();
  
  // Faster, more aggressive search pattern
  // Change search direction more frequently
  if (currentTime - lastSearch > SEARCH_TIME) {
    searchDirection = !searchDirection;
    lastSearch = currentTime;
    soundSearch(); // Soft sound while searching
  }
  
  // More efficient search: 50% turns with forward, 50% straight advance
  // Shorter cycles for quicker response
  unsigned long searchCycle = (currentTime / 150) % 8;
  
  if (searchCycle < 4) {
    // Turn while advancing forward - covers more area faster
    if (searchDirection == 0) {
      rightWithForward();
    } else {
      leftWithForward();
    }
  } else {
    // Advance straight while searching - more time advancing
    forwardMedium(); // Faster search speed
  }
}

// ------------------------------
// Sound Functions
// ------------------------------
// Sound when opponent detected
void soundOpponent() {
  tone(buzzer, 2000, 50); // Short high-pitched tone
}

// Sound when border detected
void soundBorder() {
  tone(buzzer, 1500, 100); // Medium tone
}

// Sound during search (very soft)
void soundSearch() {
  tone(buzzer, 800, 30); // Very short low tone
}

// ------------------------------
// Ultrasonic Sensor
// ------------------------------
long distance() {
  long t;
  long d;

  // Trigger pulse
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);

  // Echo duration
  t = pulseIn(echo, HIGH);

  // Time to centimeters conversion
  d = t / 59;

  // Filter unreliable values (valid range for 77cm ring)
  if (d <= MIN_VALID_DISTANCE_CM || d >= MAX_VALID_DISTANCE_CM) {
    d = INVALID_DISTANCE_CM;
  }

  // No delay for faster reading
  return d;
}

// ------------------------------
// Movement Functions
// ------------------------------
void stop() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, LOW);
}

void backward() {
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW);  // PWM
  digitalWrite(motB1, HIGH); // PWM
  digitalWrite(motB0, LOW);
}


// Fast advance (attack)
void forwardFast() {
  digitalWrite(motA1, HIGH);
  digitalWrite(motA0, LOW); // PWM
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Medium speed advance (tracking opponent)
void forwardMedium() {
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 150); // Faster tracking speed
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Slower advance (search)
void forwardSlow() {
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 100); // Search speed
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

void left() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, HIGH); // PWM
  digitalWrite(motB0, LOW);  // PWM
  digitalWrite(motB1, LOW);
}

void right() {
  digitalWrite(motA1, LOW);
  digitalWrite(motA0, LOW);  // PWM
  digitalWrite(motB0, HIGH); // PWM
  digitalWrite(motB1, LOW);
}

// Turn right while moving forward (faster search coverage)
void rightWithForward() {
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 80);  // Slower left motor
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Turn left while moving forward (faster search coverage)
void leftWithForward() {
  digitalWrite(motA1, HIGH);
  digitalWrite(motA0, LOW);
  analogWrite(motB0, 80);  // Slower right motor
  digitalWrite(motB1, LOW);
}

void backwardRight() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, HIGH);
}

void backwardLeft() {
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, LOW);
}
