const int E1 = 5; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction

const int boutonPin = 2;     // Définir la broche du bouton
int etatBouton;              // Variable pour stocker l'état du bouton
int dernierEtatBouton = LOW;  // Variable pour stocker le dernier état du bouton
unsigned long tempsAppui = 0; // Variable pour stocker le temps de début de l'appui
unsigned long debounceDelay = 50;  // Délai de rebondissement en millisecondes


void setup(void)
{
  int i;
  for(i=4;i<=5;i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600);
  Serial.println("Run keyboard control");
  digitalWrite(E1,LOW);
  pinMode(2,INPUT);
  pinMode(boutonPin, INPUT); // bouton
}
int speed = 0;
void loop() {
  etatBouton = digitalRead(boutonPin); // bouton
  if (etatBouton != dernierEtatBouton) {
    tempsAppui = millis();  // Mettre à jour le temps de début de l'appui
  }
  if ((millis() - tempsAppui) > debounceDelay) {
    // Si le bouton est pressé depuis suffisamment de temps, effectuer les actions souhaitées
    if (etatBouton == HIGH) {
      // Changer d'état, effectuer les actions nécessaires
      // Vous pouvez utiliser digitalWrite() pour contrôler une sortie
        advance(70);
    }
  else {
      advance(0);
  }
  }
  // Mettre à jour le dernier état du bouton
  dernierEtatBouton = etatBouton;
  Serial.println(dernierEtatBouton);
  delay(100);
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
