void Camera() {
  
     if ((params[4]!= NULL)&& (params[4]!=valeurs[4])) { //si une valeur est passee et differente de la valeur actuelle
        servoTo(ScamHB, valeurs[4], params[4], vitesseCam);
        Serial.println(valeurs[4]);
        Serial.println(params[4]);
        valeurs[4] = params[4];
        Serial.println(valeurs[4]);
      }//fin if

     if ((params[5]!= NULL)&& (params[5]!=valeurs[5])) { //si une valeur est passee et differente de la valeur actuelle
        servoTo(ScamRot, valeurs[5], params[5], vitesseCam);
        Serial.println(valeurs[5]);
        Serial.println(params[5]);
        valeurs[5] = params[5];
        Serial.println(valeurs[4]);
      }//fin if

}// fin void camera

void CameraFin(){
     servoTo(ScamHB, valeurs[4], 60, vitesseCam);
     valeurs[4] = 60;
     servoTo(ScamRot, valeurs[5], 60, vitesseCam);
     valeurs[5] =60;
}
//... Fonction de positionnement progressif du servomoteur pas à pas fixe de n degrés...

 void servoTo( Servo toServo, int fromAngle, int toAngle, int toVitesse)  {
  
    int delta=toAngle-fromAngle;  // variation d'angle
   // Serial.print(F("delta=")), Serial.println(delta);
    
    if (delta>=0)  {  // si variation positive
    for (int i=0; i<delta; i++) {  // défile n positions pour atteindre l'angle final dans sens positif
    
    fromAngle=fromAngle+1;  // ajoute 1 cran
    toServo.write(fromAngle); // crée une impulsion à partir valeur angle plus précis que write()
   // Serial.print("angle courant servo=");
    //Serial.println(fromAngle);
    delay(toVitesse); // pause entre chaque positionnement
    
    } // fin de for
    
  }  // fin de if
  
    else {  // si variation négative
    for (int i=-delta; i>0; i--)  {  // defile n positions pour atteindre angle final dans sens négatif
    fromAngle=fromAngle-1;  // ajoute 1 cran
    toServo.write(fromAngle);  // crée impulsion à partir valeur angle - plus précis que write()
    //Serial.print("angle courant servo=");
    //Serial.println(fromAngle);
    delay(toVitesse);  // pause entre chaque positionnement
    
    } // fin de for
    
  } // fin de else
  
} // fin de la fonction servoTo


