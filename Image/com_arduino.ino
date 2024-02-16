int x; 

void setup() { 
	Serial.begin(115200); // configure le baudrate, à adapter selon le capteur et l'usage
	Serial.setTimeout(1); 
} 
void loop() { 
	while (!Serial.available()); // permet de ne pas faire d'erreur si jamais le programme python n'est pas là tout de suite
	x = Serial.readString().toInt(); // lit le message envoyé depuis python et le convertit en entier
	Serial.print(x + 1); // affiche dans le moniteur série
} 
