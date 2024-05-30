const int E1 = 5; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction
const int trigPin = 7;
const int echoPin = 8;
float temp = 220 ;
float dernier = 50;
float dernieres[5] = {50, 50, 50, 50, 50};
float vitesseCte = 50;
float ecartCoureur = 20;
int IR_SENSOR = 0; // Sensor is connected to the analog A0
int intSensorResult = 0; //Sensor result
float fltSensorCalc = 0; //Calculated value
int courant = 0;
int linfini = 0;
float x=0 ;
float filtreBase1(float rawData, float lastFiltered, int fcoupure, float Te)
{
  /*
  Filtre linéaire numérique d'ordre 1, comme on a vu en physique CN2
  */
  float tau = 1/6.28/fcoupure;
  float filtered = (tau-Te)/tau * lastFiltered + Te/tau * rawData;
  return filtered;
}
//int combien = 5;
float moyenneGlissante(float (&lastData)[5], float nouvelleValeur, int indiceCourant)
{
  /* 
  moyenneGlissante requiert un tableau de 5 (ou autre valeur, à voir!!) précédentes valeurs 
  d'origine (donc à combiner avec la fonction passebas que tu as créée Antonio), la nouvelle valeur
  qui remplacera la plus ancienne du tableau (il s'agit d'un indice "glissant" qui devra être
  initialisé en global puis incrémenté à la fin de chaque exécution de void loop())
  */

  int indiceGlissant = indiceCourant % 5;
  lastData[indiceCourant] = nouvelleValeur;
  float somme = 0;
  int i;
  for (i = 0; i<5; i++)
  {
    somme += lastData[i];
  }
  return somme / 5;
}
/*
float filtreComplet(float (&lastData)[5], float nouvelleValeur, int indiceCourant, int &infini)
{
  
  //nécessite simplement une initialisation de l'indice courant et de l'infini à 0 tous les deux
  
  int indiceGlissant = indiceCourant % 5;
  if (nouvelleValeur > 1000 && infini < 100) // pics erratiques 
  {
    infini += 1;
    if (indiceGlissant == 0) { return  moyenneGlissante(lastData, lastData[5], indiceGlissant); } 
    else {return moyenneGlissante(lastData, lastData[indiceGlissant - 1], indiceGlissant);}
  }
  else if (infini >= 100) // ca fait plus de cent mesures que c'est à l'infini, c'est pour l'arrêter
  {
    return nouvelleValeur;
  }
  else // tous les autres cas, on filtre pour lisser la courbe
  {
    infini = 0;
    return moyenneGlissante(lastData, nouvelleValeur, indiceCourant);
  }
}
*/


float passebas(float cm , float temp, float valeurLim){

  float cmFiltre ;

  if ( cm <= valeurLim ) {
    cmFiltre = cm ;
  }
  else {
    cmFiltre = temp ;
  }

  return cmFiltre ;
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

  //float cm = mesureSonar();
  float cm = mesureIRalt();
  //float valeurLim = cm + 50 ;
  //float cmFiltre = passebas(cm , temp, valeurLim);
  float position = filtreBase1(cm, dernier, 5, 0.01);
  //float moygli = moyenneGlissante(dernieres, cm, courant);
  //float complet = filtreComplet(dernieres, cm, courant, linfini);
  Serial.print(analogRead(IR_SENSOR));
  Serial.print('\n');
  // LowPass<2> lp(6000, 100, true);

  // float cmf = lp.filt(cm);

  //float vitesse = map(complet, 5, 100, 100, 0); //modifie
  float differentiel = ecartCoureur - position;
  if (differentiel <= 0) {
    float vitesseplus = map(differentiel, 5, ecartCoureur, 40, 100);
    float vitessemoins = 0;
    advance(vitesseplus);
  }
  else {
    float vitessemoins = map(differentiel, 0, 40, 100, 40);
    float vitesseplus = 0;
    back_off(vitessemoins);
  }
  //float vitesse = map(valeurFiltree, 5, 100, 100, 0); //modifie

  Serial.print(cm);
  Serial.print("cm  ");
  //Serial.print(cmFiltre);
  //Serial.print("cmFiltre ");
  Serial.print(position);
  Serial.print(" position ");
  //Serial.print(moygli);
  //Serial.print(" moygli ");
  Serial.println();

/*
  if ( position <= 100) { // modifie
    advance(vitesse);
  }
  else {
    stop();
  }
  */
/*
  //temp = cmFiltre;
  dernier = valeurFiltree;
  courant += 1;
  */
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void advance(char a)
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);
}

void back_off (char a)
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
}

void stop()
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
  // intSensorResult = analogRead(IR_SENSOR); //Get sensor value
  // fltSensorCalc = ((6787.0 / (intSensorResult - 3.0)) - 4.0)/2.6; //Calculate distance in cm
  return cm ;
}
// Arduino Code to measure distance with a Sharp GP2D12 sensor
// www.swanrobotics.com

int mesureIR() {
  intSensorResult = analogRead(IR_SENSOR); //Get sensor value
  fltSensorCalc = ((6787.0 / (intSensorResult - 3.0)) - 4.0)/2.6; //Calculate distance in cm
  return fltSensorCalc ;
}
/* On oublie cette partie
int mesureIRalt(){
  intSensorResult = analogRead(IR_SENSOR); //Get sensor value
  x = intSensorResult ;
  //fltSensorCalc = - 79.29998511176315*x*x*x*x*x*x*x + 778.4883525654406*x*x*x*x*x*x - 3179.676321660266*x*x*x*x*x + 7010.748935796856*x*x*x*x - 9041.71916794599*x*x*x + 6882.420577339685*x*x - 2936.7161484327144*x + 595.339324606415 ;
  fltSensorCalc = x*(x*(x*(x*(x*(x*(- 79.29998511176315*x + 778.4883525654406) - 3179.676321660266) + 7010.748935796856) - 9041.71916794599) + 6882.420577339685) - 2936.7161484327144) + 595.339324606415 ;
  return fltSensorCalc ;
}
*/
