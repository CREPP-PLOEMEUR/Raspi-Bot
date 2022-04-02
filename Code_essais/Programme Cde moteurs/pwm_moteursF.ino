


// Auteur du Programme : T. LEVRON - Tous droits réservés
//  20/05/2017.

//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License,
//  or any later version.
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


//****QUE FAIT CE PROGRAMME ? **** //

//Il permet de controler les deux moteurs cc du raspibot
// a l'aide du terminal serie Arduino.

//Les commandes sont les suivantes:
// S(ou s) :commande stop pour les deux moteurs
// commande de type XYYYXYYY ou :
// YYY represente un nombre de 0 a 255
// X a pour valeur A (ou a) pour avancer, R (ou r) pour reculer
// Le terme de gauche represente le moteur gauche
// Le terme de droite represente le moteur droit
// exemples : 
// s{entree} : le robot stoppe
// A127r097 : le moteur gauche part en marche avant (avec une valeur de 127),
//            le moteur droit part en marche arriere (avec une valeur de 97)

// remarque : si vous voulez une vitesse lente,(< 100 ou <10),plusieurs syntaxes sont reconnues:
// par exemple pour une marche avant à vitesse 9 : a009=a09=A9


//  Déclaration des constantes PWM moteur(analogique)

const int VITESSE_G=11; // broche vitesse moteur gauche (E1)
const int VITESSE_D=9; // broche vitesse moteur droit (E2)

//  Déclaration des constantes moteur numeriques

const int SENS_MOTG=10; // broche sens rotation moteur gauche (M1)
const int SENS_MOTD=8; // broche sens rotation moteur droit (M2)

// Declaration constantes diverses
int moteurG=0;
int moteurD=0;
int sensG =83;
int sensD=83;


int caractere=0;
 int octetReception=0; // variable de stockage des valeurs reçues sur le port Série
 int nombreReceptionD=0; // variable de vitesse moteur droit  reçue sur le port Série
 int nombreReceptionG=0; // variable de vitesse moteur gauche  reçue sur le port Série
 long nombreReception0=0; // variable de stockage du dernier nombre  reçu sur le port Série
 String sensReception=""; // déclare un objet String vide pour reception chaine


// initialisation


void setup()   { 



// Connexion serie (l'usb utilise les broches 0 et 1) 

Serial.begin(115200); // initialise la connexion série a 115200 bauds



//Mise en sortie des broches 
 pinMode (VITESSE_G,OUTPUT); 
 pinMode (VITESSE_D,OUTPUT); 
 pinMode (SENS_MOTG,OUTPUT); 
 pinMode (SENS_MOTD,OUTPUT); 

// boucle programme
}

void loop(){ 





// réception valeur sur port Série 
        if (Serial.available()==true) { 
            recevoirMoteur(); // si caractères présents sur port Série appel de la fonction recevoirMoteur
            if (sensG==83 && sensD==83) {// si instruction stop sur les deux moteurs
               arret();
            }//fin if
       else {// sinon, si un moteur au moins est en fonctionnement
            arret();
            roule();
       }// fin else

        } // fin if serial



} // fin de la fonction loop() 


// ////////////////////////// AUTRES FONCTIONS DU PROGRAMME ////////////////////

//Instructions moteurs  sur le port série ---

void recevoirMoteur() { // fonction de reception controle moteur par le port série

        int octetRecu=0; // variable pour octet recu
        int compt=0; //  comptage caractères reçus
        int CoteMoteur=0;
        int erreur =0;
        while (Serial.available()>0) { // tant qu'un octet en réception
                
               caractere=Serial.read(); // Lit le 1er octet reçu et le met dans la variable                                           
                         
                  if  (caractere==10) { // si Octet reçu est le saut de ligne
                      compt=0;           
                      String sg=" ";
                      String sd=" ";
                      String g ="Gauche  ";
                      String d = "  Droite ";
                      
                      if (((sensD==83) or (sensD==115)) & ((sensG==83) or (sensG==115))){ 
                         Serial.println ("STOP") ;
                         break;
                      }// fin if
                      
                      else{ 
                          if ((sensG==65) or (sensG==97)){  sg="Avant ";}
                          if ((sensD==65) or (sensD==97)){  sd="Avant ";}
                          if ((sensG==82) or (sensG==114)){ sg="Recul ";}
                          if ((sensD==82) or (sensD==114)){ sd="Recul ";}                      
                          Serial.print (g + sg) ;
                          Serial.print (nombreReceptionG) ;
                          Serial.print (d + sd) ;
                          Serial.println (nombreReceptionD) ;                          
                      } // fin else
                      break;
                  }//fin if 
                       
                  else {
                        if ( (compt >1) && ((caractere==65) or (caractere==82) or (caractere==97) or (caractere==114))) { 
                           CoteMoteur=1-CoteMoteur;
                           compt=0;
                         }// fin if
                       
                        switch (compt){
              
                        case 0 :                         
                               if ((caractere == 83 ) or (caractere==115)){
                                  sensG=83;
                                  sensD=83;
                                  nombreReceptionG=0;
                                  nombreReceptionD=0;
                                  break;                                                            
                               }//fin if
                      
                               if  ((caractere==65) or (caractere==82) or (caractere==97) or (caractere==114)) { 
                          
                                   if (CoteMoteur==0) {sensG=caractere;}                                    
                                   if (CoteMoteur==1) {sensD=caractere;}
                                   compt=compt+1;
                                   break;
                                }//fin if
                                                   
                              else {
                                   if (CoteMoteur==0) {Serial.println("erreur :sens moteur gauche invalide");}
                                   if (CoteMoteur==1) {Serial.println("erreur :sens moteur droit invalide");}
                                   erreur_commande();
                                   break;   
                               } //fin else
                      
                 
                    
                        case 1 :
                               if ((octetRecu>=0)&&(octetRecu<=9)){//obtention du nombre pour la vitesse     
                                  octetRecu=caractere-48;//transformation ascii/valeur decimale
                                     if (CoteMoteur==0) {nombreReceptionG = octetRecu;} 
                                     else {nombreReceptionD = octetRecu;}     
                                compt=compt+1; 
                                break;
                                } // fin if
                        
                                else {
                                     Serial.println ("Commande invalide");
                                      erreur_commande();
                                      break;
                                } // fin else
                                
                
                        case 2 :
                               if ((octetRecu>=0)&&(octetRecu<=9)){//obtention du nombre pour la vitesse
                                  octetRecu=caractere-48;//transformation ascii/valeur decimale
                           
                                     if (CoteMoteur==0) {nombreReceptionG = (nombreReceptionG*10)+octetRecu;}                                     
                                     else {nombreReceptionD = (nombreReceptionD*10)+octetRecu;}
                                     compt=compt+1; 
                                     break;
                               } // fin if
                               else {
                                    Serial.println ("Commande invalide");
                                    erreur_commande();
                                    break;
                               } // fin else
                         
                         
                        case 3:
                              if ((octetRecu>=0)&&(octetRecu<=9)){//obtention du nombre pour la vitesse
                                 octetRecu=caractere-48;//transformation ascii/valeur decimale     
                                  
                                     if (CoteMoteur==0) {                                                                          
                                        nombreReceptionG = (nombreReceptionG*10)+octetRecu;                                    
                                        CoteMoteur=1-CoteMoteur;
                                        
                                           if (nombreReceptionG>255){
                                              Serial.println("erreur :valeur vitesse gauche>255");
                                              erreur_commande();                                     
                                              break;
                                           } // fin if
                                     } // fin if
                                     
                                     else {                              
                                          nombreReceptionD = (nombreReceptionD*10)+octetRecu;                            

                                           if (nombreReceptionD>255){
                                              Serial.println("erreur :valeur vitesse droite>255"); 
                                              erreur_commande();
                                              break;   
                                           }//fin if
                                     }// fin else                                                           
                              compt = 0;                                 
                              break;                       
                              }// fin if
                               
                              else {
                                   Serial.println ("Commande invalide");
                                    erreur_commande();
                                    break;
                              } // fin else
                               
                        
                      }// fin switch
               
                      delay(1); // pause pour laisser le temps à la fonction available de recevoir octet suivant
            
                 }//fin else
                     
          }// fin while      
          
}//fin fonction recevoir moteurs


void erreur_commande() {// fonction de traitement d'erreurs de saisies
  
          nombreReceptionD=0;
          nombreReceptionG=0;
          sensD=83;
          sensG=83; 
          caractere=10;                                 
                          
             while (Serial.available()>0) { // tant qu'un octet en réception
                   int erreur=Serial.read(); // Lit  octet reçu et le met dans la variable 
             }// fin while
}// fin fonction       

        



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
    
  analogWrite(VITESSE_G,nombreReceptionG); // impulsion PWM vitesse  gauche
  analogWrite(VITESSE_D,nombreReceptionD); // impulsion PWM vitesse  droite
    
} // fin fonction roule
    
                  
