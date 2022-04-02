//****QUE FAIT CE PROGRAMME ? **** //




void Moteurs() { // Application des parametres moteurs
              
              int i;// variable de defilement des parametres
              
              if ((params[i]!= NULL)&& (params[i]!=valeurs[i])) { //si une valeur est passee et differente de la valeur actuelle
                if ((params[2]=0) && (params[3]=0)){ // si on recoit vitesse 0 pour les deux moteurs...
                    arret();// on stoppe le robot
                } // fin if vitesse nulle
              else { // si la vitesse est non nulle pour au moins un moteur...
                
              }
                for ( i=2;i<4;i++){// pour  les valeurs de vitesse...
                     (params[i]=valeurs[i]);// on enregistre la nouvelle valeur                                         
                }
                for(i=0;i<2;i++){
                    if (params[i]=valeurs[i]) {
                      //on applique la nouvelle vitesse progressivement
                     // on enregistre la nouvelle vitesse
                    }
                    else {
                        //on met la vitesse a 0;
                        //on change le sens
                        //on applique la nouvelle vitesse
                        //on enregistre la nouvelle vitesse et le nouveau sens
                    }
                }
                }

                for( i=0;i<2;i++) { // pour les parametres sens ...              
                  // alors il y a changement de sens
                      params[i]=valeurs[i];// on stocke la valeur du nouveau sens
//                      arret();//on met le moteur à vitesse 0
                      Moteurs(); // on repart dans l'autre sens     
                  }
              }
 //             if (params[0]==valeurs[0]& params[2]==valeurs[2]){ //si le sens des moteurs  ne changent pas...
   //              Moteurs();}
//}



void MoteursFin(){
         
 }



void arret() {// fonction stop du robot
  
      digitalWrite(SENS_MOTG,LOW); // Arret moteur
      digitalWrite(SENS_MOTD,LOW); // Arret moteur
      digitalWrite(VITESSE_G,LOW); // broche PWM au niveau bas = pas d'impulsion PWM
      digitalWrite(VITESSE_D,LOW); // broche PWM au niveau bas = pas d'impulsion PWM
      delay (200);      
} //fin fonction stop


void roule() { // fonction de deplacement du robot
  
  if (sensG ==82 or sensG==114) {digitalWrite(SENS_MOTG,LOW);} // Marche arriere gauche
  if (sensG ==65 or sensG==97)  {digitalWrite(SENS_MOTG,HIGH);}// Marche avant gauche
  if (sensD ==82 or sensD==114) {digitalWrite(SENS_MOTD,LOW);} // Marche arriere droit
  if (sensD ==65 or sensD==97)  {digitalWrite(SENS_MOTD,HIGH);} // Marche avant droit 
    
  //analogWrite(VITESSE_G,nombreReceptionG); // impulsion PWM vitesse  gauche a modifier
//  analogWrite(VITESSE_D,nombreReceptionD); // impulsion PWM vitesse  droite a modifier
    
} // fin fonction roule
