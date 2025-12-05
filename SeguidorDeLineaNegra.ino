// ------------------------------
// Seguidor de Línea Negra Optimizado
// Optimizado para ring de 77cm - Búsqueda agresiva de oponente
// ------------------------------

int motB0 = 2;
int motB1 = 3; //PWM
int motA1 = 4;
int motA0 = 5; //PWM

int sensorleft = A7;
int sensorcenter = A6;
int sensorright = A5;

int buzzer = 6;
int echo = 8;
int triger = 7;

// ------------------------------
// Constantes
// ------------------------------
const int LINE_THRESHOLD = 550;        // Umbral de línea negra
const int OBSTACLE_DISTANCE = 50;      // Distancia para atacar (cm) - optimizado para ring 77cm
const int MAX_VALID_DISTANCE = 77;     // Máxima distancia válida (tamaño del ring)
const int MIN_VALID_DISTANCE = 2;      // Mínima distancia válida
const unsigned long TIEMPO_BUSQUEDA = 200; // Tiempo entre cambios de dirección de búsqueda (ms)

// ------------------------------
// Variables
// ------------------------------
int l1 = 0; // Sensor derecha
int l2 = 0; // Sensor centro
int l3 = 0; // Sensor izquierda

int f1 = 0; // Memoria sensor derecha
int f2 = 0; // Memoria sensor centro
int f3 = 0; // Memoria sensor izquierda

long cont1 = 0; // Contador de memoria

unsigned long ultimaBusqueda = 0;
int direccionBusqueda = 0; // 0=derecha, 1=izquierda
bool buscando = false;

// ------------------------------
// Setup
// ------------------------------
void setup()
{
  Serial.begin(9600);
  
  // Motores
  pinMode(motA0, OUTPUT);
  pinMode(motA1, OUTPUT);
  pinMode(motB0, OUTPUT);
  pinMode(motB1, OUTPUT);
  
  // Buzzer
  pinMode(buzzer, OUTPUT);
  
  // Sensor ultrasónico
  pinMode(echo, INPUT);
  pinMode(triger, OUTPUT);
  digitalWrite(triger, LOW);
  
  // Sensores de línea
  pinMode(sensorright, INPUT);
  pinMode(sensorcenter, INPUT);
  pinMode(sensorleft, INPUT);
  
  // Inicializar motores apagados
  alto();
}

// ------------------------------
// Loop Principal
// ------------------------------
void loop()
{
  // Leer sensores de línea
  l3 = analogRead(sensorleft);
  l2 = analogRead(sensorcenter);
  l1 = analogRead(sensorright);
  
  // Sistema de memoria cuando todos los sensores están en el mismo color
  if ((l1 > LINE_THRESHOLD && l2 > LINE_THRESHOLD && l3 > LINE_THRESHOLD || 
       l1 < LINE_THRESHOLD && l2 < LINE_THRESHOLD && l3 < LINE_THRESHOLD) && cont1 > 0)
  {
    l1 = f1;
    l2 = f2;
    l3 = f3;
    cont1 = cont1 - 1;
  }
  
  // Medir distancia al oponente
  long d = distancia();
  
  // PRIORIDAD 1: Si detecta línea negra, evita el borde
  if (l1 > LINE_THRESHOLD || l2 > LINE_THRESHOLD || l3 > LINE_THRESHOLD)
  {
    evitarBorde();
    return;
  }
  
  // PRIORIDAD 2: Si detecta oponente cerca, ataca
  if (d < OBSTACLE_DISTANCE && d >= MIN_VALID_DISTANCE)
  {
    buscando = false;
    atacarOponente();
    cont1 = 20000; // Guardar memoria
    f1 = l1;
    f2 = l2;
    f3 = l3;
    return;
  }
  
  // PRIORIDAD 3: Seguir línea si está detectada
  seguirLinea();
  
  // Guardar valores en memoria
  f1 = l1;
  f2 = l2;
  f3 = l3;
}

// ------------------------------
// Seguimiento de línea
// ------------------------------
void seguirLinea()
{
  // Línea desviada a la derecha - girar izquierda
  if (l1 < LINE_THRESHOLD && l2 > LINE_THRESHOLD && l3 > LINE_THRESHOLD || 
      l1 < LINE_THRESHOLD && l2 < LINE_THRESHOLD && l3 > LINE_THRESHOLD)
  {
    izquierdaRapida();
    cont1 = 20000;
  }
  // Línea desviada a la izquierda - girar derecha
  else if (l1 > LINE_THRESHOLD && l2 > LINE_THRESHOLD && l3 < LINE_THRESHOLD || 
           l1 > LINE_THRESHOLD && l2 < LINE_THRESHOLD && l3 < LINE_THRESHOLD)
  {
    derechaRapida();
    cont1 = 20000;
  }
  // Línea centrada - avanzar rápido
  else if (l1 > LINE_THRESHOLD && l2 < LINE_THRESHOLD && l3 > LINE_THRESHOLD)
  {
    adelanter();
    cont1 = 20000;
  }
  // Sin línea visible - buscar oponente activamente
  else
  {
    buscarOponente();
  }
}

// ------------------------------
// Evitar borde del ring
// ------------------------------
void evitarBorde()
{
  // Retroceder rápidamente
  atras();
  delay(300);
  
  // Girar para alejarse del borde
  if (l1 > LINE_THRESHOLD || l2 > LINE_THRESHOLD) 
  {
    // Línea detectada más a la derecha, girar izquierda
    izquierdaRapida();
    delay(400);
  }
  else 
  {
    // Línea detectada más a la izquierda, girar derecha
    derechaRapida();
    delay(400);
  }
  
  buscando = false; // Reiniciar búsqueda
}

// ------------------------------
// Búsqueda activa de oponente
// ------------------------------
void buscarOponente()
{
  unsigned long tiempoActual = millis();
  
  // Cambiar dirección de búsqueda rápidamente para buscar activamente
  if (tiempoActual - ultimaBusqueda > TIEMPO_BUSQUEDA)
  {
    direccionBusqueda = !direccionBusqueda;
    ultimaBusqueda = tiempoActual;
    buscando = true;
  }
  
  // Girar en la dirección actual - búsqueda agresiva
  if (direccionBusqueda == 0)
  {
    derechaRapida();
  }
  else
  {
    izquierdaRapida();
  }
}

// ------------------------------
// Ataque al oponente
// ------------------------------
void atacarOponente()
{
  // Avanzar a máxima velocidad hacia el oponente
  adelanter();
}

// ------------------------------
// Sensor ultrasónico
// ------------------------------
long distancia()
{
  long t;
  long d;
  
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
  
  t = pulseIn(echo, HIGH);
  d = t / 59;
  
  // Filtrar valores fuera del rango válido del ring
  if (d <= MIN_VALID_DISTANCE || d >= MAX_VALID_DISTANCE)
  {
    d = 1000; // Distancia inválida
  }
  
  return d;
}

// ------------------------------
// Funciones de movimiento
// ------------------------------
void alto()
{
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW);
  digitalWrite(motB0, LOW);
  digitalWrite(motB1, LOW);
}

void atras()
{
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW);
  digitalWrite(motB1, HIGH);
  digitalWrite(motB0, LOW);
}

// Avance rápido (ataque) - máxima velocidad
void adelanter()
{
  digitalWrite(motA1, HIGH);
  digitalWrite(motA0, LOW);
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Avance controlado (búsqueda)
void adelantes()
{
  digitalWrite(motA1, HIGH);
  analogWrite(motA0, 100);
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

// Giro izquierda rápido
void izquierdaRapida()
{
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, HIGH);
  digitalWrite(motB0, LOW);
  digitalWrite(motB1, LOW);
}

// Giro derecha rápido
void derechaRapida()
{
  digitalWrite(motA1, LOW);
  digitalWrite(motA0, LOW);
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

void izquierda()
{
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, HIGH);
  digitalWrite(motB0, LOW);
  digitalWrite(motB1, LOW);
}

void derecha()
{
  digitalWrite(motA1, LOW);
  digitalWrite(motA0, LOW);
  digitalWrite(motB0, HIGH);
  digitalWrite(motB1, LOW);
}

void atrasderecha()
{
  digitalWrite(motA0, LOW);
  digitalWrite(motA1, LOW);
  digitalWrite(motB0, LOW);
  digitalWrite(motB1, HIGH);
}

void atrasizquierda()
{
  digitalWrite(motA0, HIGH);
  digitalWrite(motA1, LOW);
  digitalWrite(motB0, LOW);
  digitalWrite(motB1, LOW);
}
