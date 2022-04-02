// ==== Rendre fluide le mouvement d'un servomoteur =====
// Programme Arduino par X.HINAULT - 2012 - licence GPL v3

// ....Inclusion de librairie ...
#include <Servo.h>

//... Entete déclarative = variables et constante globales ....
int octetReception=0;      // variable de réception octet
long nombreReception=0;    // déclare variable long stocke nombre reçu

//... Constantes de paramétrage du servomoteur...
const int posMin=550;      // largeur impulsion en µs correspondant à la position 0° du servomoteur
const int posMax=2500;     // largeur impulsion en µs correspondant à la position 180° du servomoteur
const int brocheServo=A4;  // broche du servomoteur
Servo servo;
int angleServo=0;          // variable de position du servomoteur
int angleServo0=0;         // variable pour mémoriser la dernière position du servomoteur
int vitesse=75;            // vitesse de positionnement entre 2 pas - en ms - entre 5 et 100 ms

//-----FONCTION SETUP ------

void setup() {             // éxécutée au début et en une seule fois

  Serial.begin(115200);                       // initialise la vitesse de la connexion série = utilise la meme vitesse dans le Terminal série
  servo.attach(brocheServo, posMin, posMax);  // attache le servomoteur à la broche
  servo.write(angleServo);                    // positionne le servomoteur à l'angle voulu initial=0°
  angleServo0=angleServo;                     // mémorise la position courante du servomoteur
  
} // Fin de la fonction setup()

// ....FONCTION LOOP ....

void loop() {              // la fonction éxécutée en boucle sans fin

  while (Serial.available()>0) {                   // si un caractère en réception
  octetReception=Serial.read();                    // lit le premier octet de la file d'attente
  
    if(octetReception==10) {                       // si octet reçu est le saut de ligne
     Serial.print("Saut de ligne recu:");
     Serial.print("Nombre recu=");                  // affiche le nombre reçu
     Serial.println(nombreReception);
     angleServo=nombreReception;
   //servo.write(angleServo);                       // positionne le servomoteur à l'angle voulu
     servoTo(servo, angleServo0,angleServo,vitesse); // ---fonction de positionnement progressif du servo pas à pas fixe de n degrés----
     angleServo0=angleServo;                        // mémorise la nouvelle position
     Serial.print("Angle=");
     Serial.print(angleServo);                      // affiche la valeur de l'angle utilise
     Serial.println("degres");
     nombreReception=0;                             // RAZ le string de reception
     delay(10);                                     // pause
     break;                                         // sort de la boucle while
    
    } // fin de if(octetReception==10)
    
    else {                                         // si le caractere reçu n'est pas un saut de ligne
      octetReception=octetReception-48;            // transformation valeur ASCII en valeur décimale - calcul du nombre à partir de valeurs reçues
      
      if((octetReception>=0)&&(octetReception<=9)) nombreReception=(nombreReception*10)+octetReception;
      else Serial.println("La chaine n'est pas un nombre valide !");
      delay(1);                                    // laisse le temps au caractere d'arriver
      
    } // Fin de else
    
  } // Fin de while
  
}  // Fin de la fonction loop()

// ------AUTRE FONCTION --------

void servoTo( Servo toServo, int fromAngle, int toAngle, int toVitesse) {
  
  int delta=toAngle-fromAngle;                       // variation d'angle
  Serial.print(F("delte=")),Serial.println(delta);
  
  if (delta>=0) {                                    // si variation positive
    for (int i=0; i<delta; i++) {                    // défine n positionss pour atteindre l'angle final dans le sens positif
    fromAngle=fromAngle+1;                           // ajoute 1 cran
    toServo.write(fromAngle);                        // crée impulsion àpartir valeur angle - plus précis que write()
    Serial.print("angle courant servo="),Serial.println(fromAngle);
    delay(toVitesse);                                // pause entre chaque positionnement
  
  } // Fin de for
  
  }// Fin de if
  
  else {                                             // si variation négative
    for(int i=-delta; i>0; i--) {                    // defile n positions pour atteindre angle final dans le sens négatif
    fromAngle=fromAngle-1;                           // retranche 1 cran
    toServo.write(fromAngle);                        // crée impulsion à partir valeur angle - plus précis que write()
    Serial.print("angle courant servo="),Serial.println(fromAngle);
    delay(toVitesse);                                // pause entre chaque positionnement
    
    } // Fin de for
    
  } // Fin de else
  
} // Fin de la fonction servoTo()
    
    
  
