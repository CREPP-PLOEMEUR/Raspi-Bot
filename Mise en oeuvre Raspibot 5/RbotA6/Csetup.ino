
void setup()   { 


//**INITIALISATION COMMUNICATION**
Serial.begin(115200); // initialise la connexion série a 115200 bauds
SerieBT.begin(9600); //initialise  bluetooth en mode transmission
                      // pour mode programmation il faut utiliser 38400

//**INITIALISATION BROCHES**
//Mise en sortie des broches 
for (int v=2;v<13;v++){
  pinMode(pins[v],OUTPUT);
} 
pinMode(13,INPUT);
pinMode(key, OUTPUT);//broche key du bluetooth (18)
pinMode(key, LOW); // broche en mode transmission (High = mode programmation)

//**INITIALISATION SERVOS**

   for (int i=0;i<6;i++){  
      objet[i] = new Servo();//creation des 6 objets servo
   }//fin for 

  for (int i=0;i<6;i++){
      objet[i]->attach(pinsServo[i], MinServo[i], MaxServo[i]); // attache les servomoteurs à leur broche 
   // et initialisation des positions extremes

      servoTo(*objet[i],params[i+4],60,toVitesse);
      //Memservos[i]=params[i+4];
}
//**INITIALISATION BRAS** 







}
