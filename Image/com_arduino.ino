long distance; 
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
	distance = Serial.readString().toInt(); 
	Serial.print(distance); 

	if (distance >= 0) {
    	float vitesseplus = map(distance, 0, 400, 40, 100);
    	float vitessemoins = 0;
    	advance(vitesseplus);
	}
	else {
    	float vitessemoins = map(distance, 0, -400, 40, 100);
    	float vitesseplus = 0;
    	back_off(vitessemoins);
	}
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