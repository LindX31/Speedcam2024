const int E1 = 3; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction
const int trigPin = 7;
const int echoPin = 8;
float temp = 220 ;
float dernier = 50;
float dernieres[5] = {50, 50, 50, 50, 50};

int courant = 0;
int linfini = 0;


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

  float cm = mesureSonar();
  //float valeurLim = cm + 50 ;
  //float cmFiltre = passebas(cm , temp, valeurLim);
  float valeurFiltree = filtreBase1(cm, dernier, 15, 0.01);
  //float moygli = moyenneGlissante(dernieres, cm, courant);
  //float complet = filtreComplet(dernieres, cm, courant, linfini);

  // LowPass<2> lp(6000, 100, true);

  // float cmf = lp.filt(cm);

  //float vitesse = map(complet, 5, 100, 100, 0); //modifie
  float vitesse = map(valeurFiltree, 5, 100, 100, 0); //modifie

  Serial.print(cm);
  Serial.print("cm  ");
  //Serial.print(cmFiltre);
  //Serial.print("cmFiltre ");
  Serial.print(valeurFiltree);
  Serial.print(" valeurFiltree ");
  //Serial.print(moygli);
  //Serial.print(" moygli ");
  Serial.println();


  if ( valeurFiltree <= 100) { // modifie
    M1_advance(vitesse);
  }
  else {
    M1_stop();
  }

  //temp = cmFiltre;
  dernier = valeurFiltree;
  courant += 1;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
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
