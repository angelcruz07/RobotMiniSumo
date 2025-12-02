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

char valorBoton;
char penultimo = ‘n’;
char antepenultimo = ‘m’;
char vali = ‘b’;

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
if(Serial.available() > 0)
{
valorBoton = Serial.read();
if((antepenultimo == ‘w’ && penultimo == ‘d’ && valorBoton == ‘h’)
|| (antepenultimo == ‘w’ && penultimo == ‘a’ && valorBoton == ‘f’))//wdh o waf
{
vali = ‘i’;
adelanter();
penultimo = ‘n’;
antepenultimo = ‘m’;
}
else if((antepenultimo == ‘s’ && penultimo == ‘d’ && valorBoton == ‘h’)
|| (antepenultimo == ‘s’ && penultimo == ‘a’ && valorBoton == ‘f’))//sdh o saf
{
vali = ‘k’;
atras();
penultimo = ‘n’;
antepenultimo = ‘m’;
}
else if((antepenultimo == ‘a’ && penultimo == ‘w’ && valorBoton == ‘t’)
|| (antepenultimo == ‘a’ && penultimo == ‘s’ && valorBoton == ‘g’))//awt o asg
{
vali = ‘j’;
izquierda();
penultimo = ‘n’;
antepenultimo = ‘m’;
}
else if((antepenultimo == ‘d’ && penultimo == ‘w’ && valorBoton == ‘t’)
|| (antepenultimo == ‘d’ && penultimo == ‘s’ && valorBoton == ‘g’))//dwt o dsg
{
vali = ‘l’;
derecha();
penultimo = ‘n’;
antepenultimo = ‘m’;
}
else if(vali == ‘i’)//Validacion no se suelta adelante
{
adelanter();
if(valorBoton == ‘t’)
{
vali = ‘b’;
alto();
}
else if(valorBoton == ‘d’) { //d
derecha();
}
else if(valorBoton == ‘a’) { //a
izquierda();
}
}
else if(vali == ‘k’)//Validacion no se suelta atras
{
atras();
if(valorBoton == ‘g’)
{
vali = ‘b’;
alto();
}
else if(valorBoton == ‘d’) { //d
atrasderecha();
}
else if(valorBoton == ‘a’) { //a
atrasizquierda();
}
}
else if(vali == ‘j’)//Validacion no se suelta izquierda
{
izquierda();
if(valorBoton == ‘f’)
{
vali = ‘b’;
alto();
}
else if(valorBoton == ‘w’) { //w
izquierda();
}
else if(valorBoton == ‘a’) { //s
atrasderecha();
}
}
else if(vali == ‘l’)//Validacion no se suelta derecha
{
derecha();
if(valorBoton == ‘h’)
{
vali = ‘b’;
alto();
}
else if(valorBoton == ‘w’) { //w
derecha();
}
else if(valorBoton == ‘a’) { //s
atrasizquierda();
}
}

//Combinaciones de dos botones:
else if(valorBoton == ‘w’) { //w
adelanter();
}
else if(valorBoton == ‘s’) { //s
atras();
}
else if(penultimo != ‘s’ && valorBoton == ‘a’) { //a
izquierda();
}
else if(penultimo != ‘s’ && valorBoton == ‘d’) { //d
derecha();
}
else if(penultimo == ‘w’ && valorBoton == ‘a’) { //wa
izquierda();
}
else if(penultimo == ‘w’ && valorBoton == ‘d’) { //wd
derecha();
}
else if(penultimo == ‘w’ && valorBoton == ‘f’) { //wf
adelanter();
}
else if(penultimo == ‘w’ && valorBoton == ‘h’) { //wh
adelanter();
}
else if(penultimo == ‘t’ && valorBoton == ‘a’) { //ta
izquierda();
}
else if(penultimo == ‘t’ && valorBoton == ‘d’) { //td
derecha();
}
else if(penultimo == ‘s’ && valorBoton == ‘a’) { //sa
atrasizquierda();
}
else if(penultimo == ‘s’ && valorBoton == ‘d’) { //sd
atrasderecha();
}
else if(penultimo == ‘s’ && valorBoton == ‘f’) { //sf
atras();
}
else if(penultimo == ‘s’ && valorBoton == ‘h’) { //sh
atras();
}
else if(penultimo == ‘g’ && valorBoton == ‘a’) { //ga
atrasderecha();
}
else if(penultimo == ‘g’ && valorBoton == ‘d’) { //gd
atrasizquierda();
}
//Espejo
else if(penultimo == ‘a’ && valorBoton == ‘w’) { //aw
izquierda();
}
else if(penultimo == ‘d’ && valorBoton == ‘w’) { //dw
derecha();
}
else if(penultimo == ‘f’ && valorBoton == ‘w’) { //fw
adelanter();
}
else if(penultimo == ‘h’ && valorBoton == ‘w’) { //hw
adelanter();
}
else if(penultimo == ‘a’ && valorBoton == ‘t’) { //at
izquierda();
}
else if(penultimo == ‘d’ && valorBoton == ‘t’) { //dt
derecha();
}
else if(penultimo == ‘a’ && valorBoton == ‘s’) { //as
atrasderecha();
}
else if(penultimo == ‘d’ && valorBoton == ‘s’) { //ds
atrasizquierda();
}
else if(penultimo == ‘f’ && valorBoton == ‘s’) { //fs
atras();
}
else if(penultimo == ‘h’ && valorBoton == ‘s’) { //hs
atras();
}
else if(penultimo == ‘a’ && valorBoton == ‘g’) { //ag
atrasderecha();
}
else if(penultimo == ‘d’ && valorBoton == ‘g’) { //dg
atrasizquierda();
}

else {
//Si se presionan más de dos botones o no se presionan los adecuados
alto();
penultimo = ‘n’;
antepenultimo = ‘m’;

}
//Actualizar la penultimo

Serial.print(antepenultimo);
Serial.print(” “);
Serial.print(penultimo);
Serial.print(” “);
Serial.print(valorBoton);
Serial.print(” “);
Serial.print(vali);
Serial.println(” “);


antepenultimo = penultimo;
penultimo = valorBoton;
/*if(valorBoton != penultimo) {
penultimo = valorBoton;*/

}
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
