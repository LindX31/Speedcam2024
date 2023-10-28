const int E1 = 3; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction
const int trigPin = 7;
const int echoPin = 8;
float temp = 220 ;
float dernier = 50;

float filtreBase1(float rawData, float lastFiltered, int fcoupure, float Te)
{
  /*
  Filtre linéaire numérique d'ordre 1, comme on a vu en physique CN2
  */
  float tau = 1/6.28/fcoupure;
  float filtered = (tau-Te)/tau * lastFiltered + Te/tau * rawData;
  return filtered;
}

float passebas(float cm , float temp, float valeurLim){

  float cmFiltre ;

  if ( cm <= valeurLim ) {
    float cmFiltre = cm ;
  }
  else {
    float cmFiltre = temp ;
  }

  return cmFiltre ;
}

void M1_advance(char Speed) ///<Motor1 Advance
{
  digitalWrite(M1,LOW);
  analogWrite(E1,Speed);
}

void M1_back(char Speed) ///<Motor1 Back off
{
 digitalWrite(M1,HIGH);
 analogWrite(E1,Speed);
}

void M1_stop()
{
  digitalWrite(M1,HIGH);
  analogWrite(E1, 0);
}

int mesureSonar()
{
  long duration, cm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  return cm ;
}

void setup() {
  ///Serial.begin(115200);
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  for(int i=3;i<6;i++)
    pinMode(i,OUTPUT);
  
}

void loop() {

  float cm = mesureSonar();
  //float valeurLim = cm + 50 ;
  //float cmFiltre = passebas(cm , temp, valeurLim);
  float valeurFiltree = filtreBase1(cm, dernier, 15, 0.01);

  float vitesse = map(valeurFiltree, 5, 100, 100, 0); //modifie

  Serial.print(cm);
  Serial.print("cm  ");
  //Serial.print(cmFiltre);
  //Serial.print("cmFiltre ");
  Serial.print(valeurFiltree);
  Serial.print(" valeurFiltree ");
  Serial.println();


  if ( valeurFiltree <= 100) { // modifie
    M1_advance(vitesse);
  }
  else {
    M1_stop();
  }

  //float temp = cmFiltre;
  float dernier = valeurFiltree;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

