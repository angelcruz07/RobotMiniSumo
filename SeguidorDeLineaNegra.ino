int motB0 = 2;
int motB1 = 3; //PWM
int motA1 = 4;
int motA0 = 5; //PWM

int sensorleft = A7;
int sensorcenter = A6;
int sensorright = A5;

long cont1 = 0;

int l1 = 0;
int l2 = 0;
int l3 = 0;

int f1 = 0;
int f2 = 0;
int f3 = 0;

int f4 = 0;
int f5 = 0;
int f6 = 0;

int buzzer = 6; //El pin al que se conecta el buzzer es el 6

int echo = 8;
int triger = 7;

int vel = 200;

void setup()
{
Serial.begin(9600);
pinMode(motA0, OUTPUT);
pinMode(motA1, OUTPUT);
pinMode(motB0, OUTPUT);
pinMode(motB1, OUTPUT);
pinMode(buzzer, OUTPUT); // Pin 6 declarado como salida
pinMode(echo, INPUT);
pinMode(triger, OUTPUT);
digitalWrite(triger, LOW);

digitalWrite(motA0, LOW);
digitalWrite(motA1, LOW);
digitalWrite(motB0, LOW);
digitalWrite(motB1, LOW);

pinMode(sensorright, INPUT);
pinMode(sensorcenter, INPUT);
pinMode(sensorleft, INPUT);
}

void loop()
{
l3=analogRead(sensorleft);
l2=analogRead(sensorcenter);
l1=analogRead(sensorright);

if ((l1 > 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 < 550 ) && cont1 > 0) //feedback when comes all sensor on white
{
l1 = f1;
l2 = f2;
l3 = f3;
cont1=cont1-1;//decremento de ciclos para salir de la memoria
}

////////////////////////////////////////////////////////////////////////////

if (l1 < 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 > 550)
{
derecha();
}
else if (l1 > 550 && l2 > 550 && l3 < 550 || l1 > 550 && l2 < 550 && l3 < 550)
{
izquierda();
}
else if (l1 > 550 && l2 < 550 && l3 > 550)
{
adelanter();
cont1 = 20000; // Numero de ciclos para mantener la memoria
}
else
{
if(l1 > 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 < 550) //Detener en caso de 3 sensores en el mismo color
{
alto();
}
}

///////////////////////////////////////////////////////////////////
f1 = l1;
f2 = l2;
f3 = l3;
}

long distancia()
{
long t;
long d;

digitalWrite(triger, HIGH);
delayMicroseconds(10);
digitalWrite(triger, LOW);

t = pulseIn(echo, HIGH);
d = t / 59;
if (d <= 1 || d >= 25)
{
d = 1000;
}
delay(10);
return d;
}

void alto()
{
digitalWrite(motA0, 0);
digitalWrite(motA1, 0);//PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}

void atras()
{
digitalWrite(motA0, 1);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB1, 1); //PWM
digitalWrite(motB0, 0);
}

void adelante()
{
digitalWrite(motA1, 1);
analogWrite(motA0, 50); //PWM
analogWrite(motB1, 50); //PWM
digitalWrite(motB0, 1);
}

void adelanter()
{
digitalWrite(motA1, 1);
digitalWrite(motA0, 0); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);
}

void adelantes()
{
digitalWrite(motA1, 1);
analogWrite(motA0, 80); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);
}

void izquierda()
{
digitalWrite(motA0, 0);
digitalWrite(motA1, 1); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}

void derecha()
{
digitalWrite(motA1,0);
digitalWrite(motA0, 0); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);

}

void atrasderecha(){
digitalWrite(motA0, 0);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 1);
}


void atrasizquierda(){
digitalWrite(motA0, 1);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}

int motB0 = 2;
int motB1 = 3; //PWM
int motA1 = 4;
int motA0 = 5; //PWM

int sensorleft = A7;
int sensorcenter = A6;
int sensorright = A5;

long cont1 = 0;

int l1 = 0;
int l2 = 0;
int l3 = 0;

int f1 = 0;
int f2 = 0;
int f3 = 0;

int f4 = 0;
int f5 = 0;
int f6 = 0;

int buzzer = 6; //El pin al que se conecta el buzzer es el 6

int echo = 8;
int triger = 7;

int vel = 200;

void setup()
{
Serial.begin(9600);
pinMode(motA0, OUTPUT);
pinMode(motA1, OUTPUT);
pinMode(motB0, OUTPUT);
pinMode(motB1, OUTPUT);
pinMode(buzzer, OUTPUT); // Pin 6 declarado como salida
pinMode(echo, INPUT);
pinMode(triger, OUTPUT);
digitalWrite(triger, LOW);

digitalWrite(motA0, LOW);
digitalWrite(motA1, LOW);
digitalWrite(motB0, LOW);
digitalWrite(motB1, LOW);

pinMode(sensorright, INPUT);
pinMode(sensorcenter, INPUT);
pinMode(sensorleft, INPUT);
}

void loop()
{
l3=analogRead(sensorleft);
l2=analogRead(sensorcenter);
l1=analogRead(sensorright);

if ((l1 > 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 < 550 ) && cont1 > 0) //feedback when comes all sensor on white
{
l1 = f1;
l2 = f2;
l3 = f3;
cont1=cont1-1;//decremento de ciclos para salir de la memoria
}

////////////////////////////////////////////////////////////////////////////

if (l1 < 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 > 550)
{
derecha();
}
else if (l1 > 550 && l2 > 550 && l3 < 550 || l1 > 550 && l2 < 550 && l3 < 550)
{
izquierda();
}
else if (l1 > 550 && l2 < 550 && l3 > 550)
{
adelanter();
cont1 = 20000; // Numero de ciclos para mantener la memoria
}
else
{
if(l1 > 550 && l2 > 550 && l3 > 550 || l1 < 550 && l2 < 550 && l3 < 550) //Detener en caso de 3 sensores en el mismo color
{
alto();
}
}

///////////////////////////////////////////////////////////////////
f1 = l1;
f2 = l2;
f3 = l3;
}

long distancia()
{
long t;
long d;

digitalWrite(triger, HIGH);
delayMicroseconds(10);
digitalWrite(triger, LOW);

t = pulseIn(echo, HIGH);
d = t / 59;
if (d <= 1 || d >= 25)
{
d = 1000;
}
delay(10);
return d;
}

void alto()
{
digitalWrite(motA0, 0);
digitalWrite(motA1, 0);//PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}

void atras()
{
digitalWrite(motA0, 1);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB1, 1); //PWM
digitalWrite(motB0, 0);
}

void adelante()
{
digitalWrite(motA1, 1);
analogWrite(motA0, 50); //PWM
analogWrite(motB1, 50); //PWM
digitalWrite(motB0, 1);
}

void adelanter()
{
digitalWrite(motA1, 1);
digitalWrite(motA0, 0); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);
}

void adelantes()
{
digitalWrite(motA1, 1);
analogWrite(motA0, 80); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);
}

void izquierda()
{
digitalWrite(motA0, 0);
digitalWrite(motA1, 1); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}

void derecha()
{
digitalWrite(motA1,0);
digitalWrite(motA0, 0); //PWM
digitalWrite(motB0, 1); //PWM
digitalWrite(motB1, 0);

}

void atrasderecha(){
digitalWrite(motA0, 0);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 1);
}


void atrasizquierda(){
digitalWrite(motA0, 1);
digitalWrite(motA1, 0); //PWM
digitalWrite(motB0, 0); //PWM
digitalWrite(motB1, 0);
}
