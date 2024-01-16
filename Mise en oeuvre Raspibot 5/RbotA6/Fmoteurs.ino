//****MODULE MOTEURS **** //
//Il met en oeuvre les parametres recus par voie serie
//et stocke dans les quatre premieres valeurs du tableau "params"
//(details dans communication)

//ATTENTION :Pour le passage de parametres en manuel par le port serie,le module
//n'integre pas de gestion des erreurs de frappe.Le but final etant de passer les
// parametres de facon automatique ,notamment par le biais d'une interface python.

void Moteurs() { // Application des parametres moteurs
   
     static int pinSens[2]={4,7};//broches pour ecriture digitalWrite
     static int diff[2];//tableau incrementation progressive des vitesses pour chaque moteur
     int v[2];   
          for(int k=0;k<2;k++) {// pour les deux moteurs (a supprimer si un seul moteur.
                                // sinon k<nombre de moteurs
                               // Serial.println(sens[k]);
                               // Serial.println(params[k+2]);
             if(sens[k]!=params[k+2]){//Si changement de sens
              Serial.println("changement de sens");

                analogWrite(pins[k+5],0);//vitesse[k]);//on applique la vitesse nulle
                params[k] = 0;
                Serial.println("vitesse 0");
                // a adapter selon la broche vitesse choisie
                params[k+2]= sens[k];//on memorise le nouveau sens
                //Serial.println(params[k+2]);
                digitalWrite(pinSens[k],sens[k]);// on applique le nouveau sens                         
             }//fin if
             
              if(vitesse[k] < params[k]){//en cas de vitesse decroissante
                Serial.println("ralentit");
               
                 analogWrite(pins[k+5],vitesse[k]);//on met la nouvelle vitesse sans ralentissement progressif
                 // a adapter  :k + No de broche le plus faible si les broches moteur se suivent.
                 //Si les broches ne se suivent pas,creer un tableau avec leurs numeros 
                 // pour pouvoir les appeler.(cf pinSens)
                 params[k]=vitesse[k]; 
                 //Serial.println("vitessemini");                                     
               }//fin if V decroissante
                else{
                diff[k] =(vitesse[k] - params[k])/10;// on cree 10 pas /pour chaque moteur
                Serial.println("accelere");
                //Serial.print("valeur du pas ");
                //Serial.println(diff[k]);
                }//fin else                
           }//fin for k
             
               
           for (int i =0;i<10;i++) { //on applique 10 fois ce pas pour chaque moteur)
                for(int m=0;m<2;m++){// alterne l'acceleration sur chaque moteur           
                      if(diff[m]!=0){//pour un pas non nul
                        
                      v[m]=params[m] + (i+1)*diff[m];//a chaque boucle on incremente la vitesse du pas
                     // Serial.println (v[m]);
                      //alternativement sur chaque moteur                                       
                      if (v[m]>255){v[m]=255;}//verifie qu'on reste<255
                      //if (vitesse[m]<0){vitesse[m]=0;}// verifie qu'on reste>0
                      analogWrite(pins[m+5],v[m]);//application de la vitesse
                      delay(40);//pour progressivite de l'acceleration
                      
                      if(i==9){//en fin de boucle
                         params[m]=v[m];//on memorise la nouvelle vitesse
                         diff[m]=0;
                        // Serial.println(params[m]);
                         //Serial.println(v[m]);            
                      }//fin if memo vitesse
                  
                      }//fin if diff                      
                    }// fin for m
                                                                
                 }//fin for i
          
         // }// fin if "s'il y a un changement d'etat moteur
}// fin void moteurs






