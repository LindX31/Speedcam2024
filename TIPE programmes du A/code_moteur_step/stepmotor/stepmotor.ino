// Inclut la bibliothèque AccelStepper:

#include <AccelStepper.h>

 

// Définitions des broches du moteur:

#define motorPin1  8      // IN1 on the ULN2003 driver

#define motorPin2  9      // IN2 on the ULN2003 driver

#define motorPin3  10     // IN3 on the ULN2003 driver

#define motorPin4  11     // IN4 on the ULN2003 driver

 

// Définit le type d'interface AccelStepper; Moteur 4 fils en mode demi-pas:

#define MotorInterfaceType 8

 

// Initialisation avec la séquence de broches IN1-IN3-IN2-IN4 pour utiliser la bibliothèque AccelStepper avec le moteur pas à pas 28BYJ-48:

AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {

// Définit le nombre maximum de pas par seconde:

  stepper.setMaxSpeed(1000);

}

 

void loop() {

// Réglez la vitesse du moteur en pas par seconde:

  stepper.setSpeed(500);

// Faire avancer le moteur avec une vitesse constante définie par setSpeed ​​():

  stepper.runSpeed();

}