//MODULE COMMUNICATION PORT SERIE
// cette fonction permet de transmettre une rafale de parametres à Arduino sous forme d'un tableau
//Actuellement,10 parametres peuvent etre transmis.Si on ne tape que 4 parametres,ils correspondront 
//aux parametres moteurs(voir les parametres tableau dans Constantes communication)

//valeurs des parametres transmis:
//param 0:sens moteur gauche.        0 en avant 1 en arriere
//param 1:sens moteur droit .        0 en avant 1 en arriere
//param 2:vitesse PWM moteur gauche (min : max: )
//param 3:vitesse PWM moteur droit  (min : max: )
//param 4:rotation camera.           angle min 0 ;angle max: 120?
//param 5:Elevation camera           angle min 0 ; angle max:120?
//param 6:Rotation bras              angle min 0 ; angle max:120?
//param 7:Elevation Bras             angle min 0 ; angle max:120?
//param 8:Avant/arriere Bras         angle min 0 ; angle max:120?
//param 9:Pince Bras                 angle min 0 ; angle max:120?

void ComSerie() {

  while ((Serial.available() > 0)) {   //tant qu'on recoit quelque chose...
          caractere = Serial.read();//...on le lit
                
           if (caractere == 10){// si c'est un saut de ligne(fin de sequence)
             
               params[i]=nombreReception;// le nombre precedemment stocké est le dernier du tableau
               nombreReception = NULL;// remise a zero des nombres recus 
                for (int j=0;j<10;j++){
              Serial.print("params : ");
            Serial.println(j);
              Serial.println(params[j]);
              }// fin for   
               i=0;   // remise a zero du compteur de parametres recus    
               break;// sortie de la boucle
           } // fin if saut de ligne
       
          if (caractere == 44){// si c'est une virgule....
              
             if (stock==0){//si le caractere precedent n'etait pas une virgule
                params[i]= nombreReception;//on affecte la valeur precedente recue au tableau
                nombreReception = NULL;// on remet a zero la variable nombre recu
                stock=1;//on memorise le fait d'avoir recu une virgule
                i = i+1;// on passe a l indice parametre suivant
               }
              else{//sinon (si le caractere recu precedemment etait deja une virgule
                   //c'est qu'on saute la valeur(non transmise)
                i=i+1;// on passe a l'indice parametre suivant
              }// fin else
              
           }// fin if virgule       
          
       
          if ((caractere>47) & (caractere<57)){// si on recoit un chiffre...
            
              stock=0;//on memorise que l'on ne recoit pas une virgule
              caractere = caractere-48;//on transforme le chiffre ASCII en decimal
              nombreReception = (nombreReception*10)+caractere;// formule de reconstitution du nombre transmis
       
              delay(1);//laisse le temps au caractere suivant d'arriver
          }// fin if caractere

     
    }// fin while
  }//fin void comSerie
  
  
