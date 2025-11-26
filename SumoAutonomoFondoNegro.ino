// ------------------------------
// Configuración de pines
// ------------------------------
int motB0 = 2;
int motB1 = 3; // PWM
int motA1 = 4;
int motA0 = 5; // PWM

int sensorleft   = A7;
int sensorcenter = A6;
int sensorright  = A5;

int buzzer = 6; // Pin al que se conecta el buzzer (actualmente no se usa)

int echo   = 8;
int triger = 7;

// ------------------------------
// Constantes de funcionamiento
// ------------------------------
const int  LINE_THRESHOLD        = 550;  // Umbral de detección de línea negra
const int  OBSTACLE_DISTANCE_CM  = 150;  // Distancia límite para considerar obstáculo (en cm)
const long MIN_VALID_DISTANCE_CM = 1;    // Distancia mínima válida del sensor ultrasónico
const long MAX_VALID_DISTANCE_CM = 25;   // Distancia máxima válida del sensor ultrasónico
const long INVALID_DISTANCE_CM   = 1000; // Valor usado cuando la medida no es confiable

// ------------------------------
// Variables de estado
// ------------------------------
long cont1 = 0; // No se usa actualmente, se mantiene por compatibilidad

int l1 = 0; // Sensor derecha
int l2 = 0; // Sensor centro
int l3 = 0; // Sensor izquierda

// Variables reservadas (no usadas, se mantienen para compatibilidad)
int f1 = 0;
int f2 = 0;
int f3 = 0;
int f4 = 0;
int f5 = 0;
int f6 = 0;

int vel = 200; // No se usa directamente, se mantiene por compatibilidad

// ------------------------------
// Prototipos de funciones
// ------------------------------
void leerSensoresLinea();
bool hayLineaDetectada();
void maniobraEvitarBorde();
long distancia();

void alto();
void atras();
void adelante();
void adelanter();
void adelantes();
void izquierda();
void derecha();
void atrasderecha();
void atrasizquierda();

// ------------------------------
// Setup
// ------------------------------
void setup() {
  Serial.begin(9600);

  // Pines motores
  pinMode(motA0, OUTPUT);
  pinMode(motA1, OUTPUT);
  pinMode(motB0, OUTPUT);
  pinMode(motB1, OUTPUT);

  // Buzzer (no utilizado actualmente)
  pinMode(buzzer, OUTPUT);

  // Ultrasonido
  pinMode(echo, INPUT);
  pinMode(triger, OUTPUT);
  digitalWrite(triger, LOW);

  // Sensores de línea
  pinMode(sensorright, INPUT);
  pinMode(sensorcenter, INPUT);
  pinMode(sensorleft, INPUT);

  // Motores inicialmente apagados
  alto();
}

// ------------------------------
// Loop principal
// ------------------------------
void loop() {
  // Leer sensores de línea
  leerSensoresLinea();

  // Medir distancia al obstáculo delantero
  long d = distancia();

  if (d < OBSTACLE_DISTANCE_CM) {
    // Hay obstáculo dentro del rango definido
    if (hayLineaDetectada()) {
      // Si detecta línea negra, realiza maniobra de evasión
      maniobraEvitarBorde();
    } else {
      // Si no hay línea, avanza rápido hacia el oponente
      adelanter();
    }
  } else {
    // No hay obstáculo cerca, se mueve con velocidad de búsqueda
    if (hayLineaDetectada()) {
      maniobraEvitarBorde();
    } else {
      adelantes();
    }
  }
}

// ------------------------------
// Lectura de sensores de línea
// ------------------------------
void leerSensoresLinea() {
  // Orden conservada: l3 izquierda, l2 centro, l1 derecha
  l3 = analogRead(sensorleft);
  l2 = analogRead(sensorcenter);
  l1 = analogRead(sensorright);
}

// Devuelve true si cualquiera de los sensores supera el umbral de línea
bool hayLineaDetectada() {
  return (l1 > LINE_THRESHOLD || l2 > LINE_THRESHOLD || l3 > LINE_THRESHOLD);
}

// ------------------------------
// Maniobras compuestas
// ------------------------------
// Secuencia usada en el código original para evitar la línea:
// alto -> atrás -> giro a la derecha
void maniobraEvitarBorde() {
  alto();
  delay(1000);
  atras();
  delay(1000);
  derecha();
  delay(700);
}

// ------------------------------
// Sensor ultrasónico
// ------------------------------
long distancia() {
  long t;
  long d;

  // Pulso de disparo (trigger)
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);

  // Duración del eco
  t = pulseIn(echo, HIGH);

  // Conversión de tiempo a centímetros (misma fórmula original)
  d = t / 59;

  // Filtrado de valores no confiables (misma lógica original)
  if (d <= MIN_VALID_DISTANCE_CM || d >= MAX_VALID_DISTANCE_CM) {
    d = INVALID_DISTANCE_CM;
  }

  delay(10);
  return d;
}

// ------------------------------
// Funciones de movimiento
// ------------------------------
void alto() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, LOW);
}

void atras() {
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW);  // PWM
  digitalWrite(motB1, HIGH); // PWM
  digitalWrite(motB0, LOW);
}

// No se usa en el loop actual, se mantiene para compatibilidad
void adelante() {
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 50); // PWM
  analogWrite(motB1, 50); // PWM
  digitalWrite(motB0, HIGH);
}

// Avance rápido (ataque)
void adelanter() {
  digitalWrite(motA1, HIGH);
  digitalWrite(motA0, LOW); // PWM
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Avance más lento (búsqueda)
void adelantes() {
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 80); // PWM
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

void izquierda() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, HIGH); // PWM
  digitalWrite(motB0, LOW);  // PWM
  digitalWrite(motB1, LOW);
}

void derecha() {
  digitalWrite(motA1, LOW);
  digitalWrite(motA0, LOW);  // PWM
  digitalWrite(motB0, HIGH); // PWM
  digitalWrite(motB1, LOW);
}

void atrasderecha() {
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, HIGH);
}

void atrasizquierda() {
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW); // PWM
  digitalWrite(motB0, LOW); // PWM
  digitalWrite(motB1, LOW);
}
