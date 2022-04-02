
void setup()   { 


//**INITIALISATION COMMUNICATION**
Serial.begin(115200); // initialise la connexion série a 115200 bauds


//**INITIALISATION MOTEURS**
//Mise en sortie des broches 
 pinMode (VITESSE_G,OUTPUT); 
 pinMode (VITESSE_D,OUTPUT); 
 pinMode (SENS_MOTG,OUTPUT); 
 pinMode (SENS_MOTD,OUTPUT); 


//**INITIALISATION CAMERA**
 pinMode (CameraHB,OUTPUT); 
 pinMode (CameraRot,OUTPUT); 

ScamHB.attach(CameraHB, MinHB, MaxHB); // attache le servomoteur à la broche 
   // et initialisation des positions extremes
ScamRot.attach(CameraRot, MinRot, MaxRot); // attache le servomoteur à la broche 
   // et initialisation des positions extremes
 
//**INITIALISATION BRAS** 
 pinMode (BPince,OUTPUT); 
 pinMode (BAvAr,OUTPUT); 
 pinMode (BElev,OUTPUT); 
 pinMode (BRot,OUTPUT); 






}
