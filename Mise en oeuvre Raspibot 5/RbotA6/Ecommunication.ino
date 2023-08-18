//MODULE COMMUNICATION PORT SERIE
// ce module permet de transmettre une rafale de parametres à Arduino sous forme d'un tableau
//Actuellement,10 parametres peuvent etre transmis.Si on ne tape que 4 parametres,ils correspondront 
//aux parametres moteurs

//valeurs des parametres transmis:

//param 0:vitesse PWM moteur gauche (min :0 max:255 )
//param 1:vitesse PWM moteur droit  (min :0 max:255 )
//param 2:sens moteur gauche.        0 en avant 1 en arriere
//param 3:sens moteur droit .        0 en avant 1 en arriere
//param 4:rotation camera.           angle min 0 ;angle max: 120?
//param 5:Elevation camera           angle min 0 ; angle max:120?
//param 6:Rotation bras              angle min 0 ; angle max:120?
//param 7:Elevation Bras             angle min 0 ; angle max:120?
//param 8:Avant/arriere Bras         angle min 0 ; angle max:120?
//param 9:Pince Bras                 angle min 0 ; angle max:120?

void ComSerie() {
       
        int caractere;// caractere unitaire recu sur le port serie
        static int repere=0;;//detection indice ou valeur       
        int nombreReception = NULL;//nombre (parametre) en reception
        int compt=0;
  while ((Serial.available() > 0)) {   //tant qu'on recoit quelque chose...
        
        caractere = Serial.read();//...on le lit
               
           if (caractere == 10){// si c'est un saut de ligne(fin de sequence)
            
                   // Serial.print("la valeur est :");
                   // Serial.println(nombreReception);
                if (indice>3){
                   //params[indice]=nombreReception;// le nombre precedemment stocké est le dernier du tableau
                   valeur = nombreReception;
                   nombreReception = NULL;// remise a zero des nombres recus   
                   repere = 0;   // remise a zero du compteur de parametres recus 
                   Exec = 2;
                   Servos();
                   break;
                   }

               if ((indice==2)or(indice==3)){
                  sens[indice-2] = nombreReception;
                  Serial.print ("indice "); 
                  Serial.print(indice);
                  Serial.print ( " sens ") ; 
                  Serial.println(sens[indice-2]);                
                  nombreReception = NULL;
                  repere =0;
                  break;
                  }// fin if 0 ou 1

               if (indice ==0){
                  vitesse[0] = nombreReception;
                  //Serial.print ( "l'indice est ") ; 
                  //Serial.println (indice);
                  Serial.print ("moteur gauche "); 
                  Serial.println(vitesse[0]);
                  nombreReception = NULL;
                  repere =0;
                  Exec =1;
                  Moteurs();
                  break;  
                 }// fin if 2
              
               if (indice==1){
                   vitesse[1] = nombreReception;
                  //Serial.print ( "l'indice est ") ; 
                  //Serial.println (indice);
                   Serial.print ( "Moteur droit ") ;                  
                   Serial.println(vitesse[1]);
                   nombreReception = NULL;
                   repere =0;                                                    
                   break;
                   }
                              
           } // fin if pour saut de ligne
           

          if ((caractere>47) & (caractere<58)){// si on recoit un chiffre...
              if (repere == 0){           
                  indice = caractere-48;//on transforme le chiffre ASCII en decimal
                  repere=1;
                 // Serial.print("l'indice est ");
                 // Serial.println (indice);
                  break;
                  //nombreReception=0;
                  delay(1);}
              else {
                  caractere = caractere-48;//on transforme le chiffre ASCII en decimal            
                  nombreReception = (nombreReception*10)+caractere;// formule de reconstitution du nombre transmis
                  delay(1);
              }
          }// fin if caractere
      
          
         // if (caractere == 102){
          //  ServosFin();
         // }

          if ((caractere == 65) or (caractere == 97)){ //A ou a
            memauto = automatic;
            automatic =1;
            
          }

          if ((caractere == 77) or (caractere == 109)){ //M ou m
            if (memauto != automatic){
            params[0]=0;
            params[1]=0,
            Moteurs();
            automatic =0;
            memauto=0;
          }
          }  


     
    }// fin while
  }//fin void comSerie
  
  
