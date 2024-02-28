int distance; 
const int E1 = 5; ///<Motor1 Speed
const int M1 = 4; ///<Motor1 Direction
const int trigPin = 7;
const int echoPin = 8;
float temp = 220 ;
float dernier = 50;

void setup() { 
	Serial.begin(9600); // configure le baudrate, à adapter selon le capteur et l'usage
	Serial.setTimeout(1); 
} 
void loop() { 
	while (!Serial.available()); 
	delay(100); //délai nécessaire pour que la carte aie le temps d'acquérir l'information du port série
	distance = Serial.readString().toInt(); 
	//Serial.print(distance); 

	if (distance >= 0) {
    	long vitesseplus = map(distance, 0, 200, 40, 100);
    	int vitessemoins = 0;
      Serial.print(vitesseplus);
    	advance(vitesseplus);
	}
	else {
    	int vitessemoins = map(-distance, 0, 200, 40, 100);
    	int vitesseplus = 0;
      Serial.print(-vitessemoins);
    	back_off(vitessemoins);
	}
} 



void advance(int a) //modifié, à voir si on remet char à la place de int
{
  analogWrite(E1,a);
  digitalWrite(M1,HIGH);
}

void back_off (int a) //idem 
{
  analogWrite(E1,a);
  digitalWrite(M1,LOW);
}

void stop()
{
  digitalWrite(M1,HIGH);
  analogWrite(E1, 0);
}