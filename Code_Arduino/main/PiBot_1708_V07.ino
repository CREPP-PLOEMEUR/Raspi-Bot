// Auteur du Programme : Luc DOMANGE - Tous droits réservés
//  31/07/2017.

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

//____________________________________________________________________________________
//                                                                                    !
// CE PROGRAMME EST RÉALISÉ POUR PILOTER DEUX MOTO-RÉDUCTEURS A CC DU RASPIBOT        !
// D'UNE FAÇON FLUIDE AUTANT QUE POSSIBLE AFIN DE SOULAGER CETTE PETITE PIGNONERIE EN !
// COMPORTANT DES RAMPES D'ACCÉLÉRATIONS ET DE MÉMORISATION DES PARAMÈTRES            !
// AFIN DE PASSER D'UN ÉTAT CE COMMANDE À UN AUTRE EN RESTANT TOUJOURS FLUIDE.        !
//____________________________________________________________________________________!

// Hypothèse : les moteurs sont branchés - en cablage- de telle sorte que la commande LOW ou 0 (zéro)
// fassent tourner les deux moteurs pour une marche Avant suivant la normalisation 
// du tutorial "Motor Control Pins" du driver L298, page 19/22 du "moteur-et-Cde-moteur-CC-2.pdf".

// Afin de facilité l'utilisation d'un connecteur 4 broches, sur l'Arduino, il serait souhaitable d'utiliser les pines
// 6 et 7 pour les commandes PWM (0 à 255)
// 8 et 9 pour les commandes numériques de sens de rotation (0 - 1) !
// Ici, les pines 10, 11, 12 et 13 sont utilisées pour des leds d'aide à la mise au point et le fun.
// Ne clignotent pas encore en fonction de la vitesse !

// Pour plus de fluidité, la base du programme a été de choisir une vitesse nominale par rapport au centre de gravité de l'essieu.
// Cette vitesse nominale est accélérée par incréments  de 25, (ou 5 ou 1 à venir) points sur 255 suivant le choix.
// Dans ce cas, la vitesse est incrémentée et est mémorisé le cas échéant.
// Cette vitesse est utilisée quelque soit le mouvement "Avant,", "Arrière", "Tout Droit", "Vire", "Toupie".

// THIERRY a dégrossi le travail par son programme.
// L'avantage de pratiquer de la sorte en gardant constemment en mémoire la vitesse nominale est de permettre des commandes fluides 
// et de ne pas avoir à choisir des valeurs numériques aléatoires qui seraient brutales pour la mécanique.

// La vitesse des moteurs est relié sur la vitesse nominale par un rapport "rX" , soit (vitesseX = vitesse_nominale * rX).
// Dans tous les cas, les vitesses moteurs restent entre 0 et 255.

// En ligne droite r=1, 
// Pour virer, la vitesse d'un moteur est augmentée d'autant qu'elle est diminuée pour l'autre
// par le produit (rG=rG * 1.1) et (rD = rD * 0.9) ou inverssement.
// Dans ce cas, plus on vire, plus r est multiplié à la puissance n pour 1.1 et 0.9 
// ou inverssement pour redresser et tourner dans l'autre direction.
// Les rX restent toujours mémorisés quel que soit le sens de rotation des moteurs.

// Pour sortir d'un virage, la fonction "Tout Droit" remet donc le rapport r = 1 tout en gardant la meme vitesse nominale.

// Une fonction "Toupie" est installée par rapport au centre de gravité de l'essieu.

// Une fonction "Impulsion" permet un accostage doux pour la préhention d'objets.
// Cette fonction est valide au dessous d'un seuil de vitesse basse, garde le sens ainsi que le rapport r donc la courbure du virage.

// Et évidemment la fonction "STOP" placée en première fonction met V à 0, le driver L298 n'est donc plus sollicité.

// Construction du programme : Reprendre les définitions des clefs de commandes sur les Notes S_Pen du SAMSUNG LDO
// Des lettres sont affectées pour les différentes actions écritent clairement dans le programme.
// Le clavier numérique est également utilisé en parallèle et bien défini dans le programme
// afin d'avoir toutes les clefs en un seul endoit.

// La communication serie est réalisée par un Serial.read et envoye le ou les caractères frappés par un "Line Feed",
// ce qui est génant pour une bonne fluidité.
// A modifier afin d'envoyer au fur et à mesure de la frappe.

// Cette version 5 utilise le terminal de l'Arduino directement par le cable 
// ou par la commande wifi à l'aide du RaspberryPI et VNC.
// Une version à venir utilisera pyqt4 en commandes graphiques


// Une version à venir en Python avec pyqt4!
// Programmes *.ui, *.py, *Main.py, combinant clavier ASCII, et graphique combos et curseurs etc...


// 170722, version 1, construction de la structure du programme afin de créer des fonctions répétitives simples, "void".
// 170723, Cette version 2 englobe la communication avec le terminal et d'une façon fluide 
// pour une ligne droite AV et AR, + vite et - vite (accélération gamma) et STOP.
// 170725, version 3, ajout des fonctions VIRAGES "Vire Gauche", "Vire Droite", rG et rD étant déjà installés dans le calcul des vitesses,
// ajout de la fonction Toupie.
// 170726, version 5, ajout pavé numérique pour plus de facilité des comandes mouvements sous une main.
// Ajout de la fonction "Impulsion".
// Mise en forme.
// 170730 Version 6, ajout de fonctions venant des touches actionnées dans while-if pour plus de flexibilités.
// et "ATTENDRE" dans l'état, c'est à dire, attendre x_milisecondes avant de valider une nouvelle commande.
// 170823 Version 7, mises au point V06 plus RAZ Vitesses
//__________________________________________________________________
//
//**********************  D E C L A R A T I O N S   ****************

//  Déclaration des constantes moteur numériques
const int E1 = 5; // cde vitesse PWM moteur de Gauche (6 futur)
const int E2 = 6; // cde vitesse PWM moteur de Droite (7  futur)

const int M1 = 4; //moteur de Gauche, cde LOW pour marche Avant, cde HIGH pour marche Arrière (8 futur)
const int M2 = 7; //moteur de Droite, cde LOW pour marche Avant, cde HIGH pour marche Arrière (9 futur)
//                 LOW ou 0, HIGH ou 1

//  Déclaration des leds
int ledAVG = 10;
int ledARG = 11;
int ledAVD = 12;
int ledARD = 13;

int swledAVG; //switch led AVG
int swledAVD; //switch led AVD
int swledARG; //switch led AVG
int swledARD; //switch led AVD

//Déclaration des paramètres pour les moteurs
int vit_nom = 0;   //vitesse nominale, limitée de 0 a 255
int vit_nomM;
int vit_G;  //vitesse moteur de Gauche, limitée de 0 a 255
int vit_D;  //vitesse moteur de Droite, limitée de 0 a 255

char Ordre_Avant;
char Ordre_Arriere;
char Stop;

int g_plus = 25;   //relation avec accélère  (acc)
int g_moins = 25;  //relation avec décélère (dec)
int acc;     // accélère
int dec;     //décélère
int i;
int delta;
  
int sens_G;  // sens de rotation moteur Gauche (0 AV, 1 AR)
int sens_D;  // sens de rotation moteur Droit  (0 AV, 1 AR)


float rD = 1;      //rapport vitesse moteur Droite / vitesse nominale
float rG = 1;      //rapport vitesse moteur Gauche / vitesse nominale
               //NOTA : la vitesse de la roue exter ne dépassera jamais la référence maxi 255
float rGM = rG;  // mémorisation rG en cours
float rDM = rD;  // mémorisation rD en cours               

float vit_rG; // vit_rG = vit_nom * rG;
float vit_rD; // vit_rD = vit_nom * rD;
  
int t;   // pour le temps du delay de pause,   (plus nécessire)
int t2;  // pour le temps a vitesse constante, (plus nécessire)

int caractere=0;

//int caract; pas utilisé

int x_milisecondes = 500;
int arret_moment = 1;

//******************************************************************
//__________________________________________________________________
//
//****************************  S E T   U P  ***********************
// initialisation
void setup() {
  
  // Connexion serie (l'usb utilise les broches 0 et 1) 
  Serial.begin(115200); // initialise la connexion série a 115200 bauds
  
  //Mise en sortie des broches 
  pinMode (M1, OUTPUT); // sens moteur de Gauche,
  pinMode (M2, OUTPUT); // sens moteur de Droite
  
  pinMode (ledAVD, OUTPUT);
  pinMode (ledARD, OUTPUT);
  pinMode (ledAVG, OUTPUT);
  pinMode (ledARG, OUTPUT);
  
  Serial.println("                    --- DEFINITION DES TOUCHES ---");
  
  Serial.println(" ");
  Serial.println("                TABLEAU COMME DANS LES ANCIENS JEUX !");
  Serial.println(" ");
  /*
  Serial.println("STOP - ARRET    : S, s,     0 ");
  Serial.println("Avant           : A,        8 ");
  Serial.println("Arriere         : R,        2");
  Serial.println("Impulsion       :           . dot ");
  Serial.println("Accelere        : P, p, +,  9 "); // P = 25, p = 5, + = 1pt
  Serial.println("Decelere        : M, m, -,  7 "); // idem
  Serial.println("Vire Gauche     : <,        4 ");
  Serial.println("Vire Droite     : >,        6 ");
  Serial.println("Tout Droit      : D, d,     5 ");
  Serial.println("Toupie Gauche   : t,        1");
  Serial.println("Toupie Droite   : T,        3");
  */
  
  Serial.println("                 Attendre        : W, w      /");
  Serial.println("                 Arret           : Space barre ");
  Serial.println("                 Redemare        : Space barre a nouveau");
    
  Serial.println(" ");
  
  Serial.println("!-------------------------------------------------------------------------!");
  Serial.println("!                          EN CLAVIER NUMERIQUE                           !");
  Serial.println("!------------------!-----------------!--------------------!---------------!");
  Serial.println("!       nul        ! / : Attendre    ! * : RAZ Vitesses   ! - :Moins Vite !");
  Serial.println("!------------------!-----------------!--------------------!---------------!");
  Serial.println("! 7 : Decelere     ! 8 : Avant       ! 9 : Accelere       ! + :Plus Vite  !");
  Serial.println("!------------------!-----------------!--------------------!               !");
  Serial.println("! 4 : Vire Gauche  ! 5 : Tout Droit  ! 6 : Vire Droite    !               ! ");
  Serial.println("!------------------!-----------------!--------------------!---------------!");
  Serial.println("! 1 : Toupi Gauche ! 2 : Arriere     ! 3 : Toupie Droite  !      Entr :   !");
  Serial.println("!------------------------------------!--------------------!               !");
  Serial.println("! 0 :         STOP - ARRET           ! . : Impulsion      !   Line Feed   !");
  Serial.println("!------------------------------------!--------------------!---------------!");
  
  Serial.println("!  SPACE BARE :            ARRET      -      REDEMARE                     !");
  Serial.println("!-------------------------------------------------------------------------!");
  
  
}  // fin setup

//******************************************************************
//__________________________________________________________________
//
//*******************************   L O O P   **********************
// boucle du programme
void loop() {
  
  //Serial.println("!-------------------------------------------------------------------------!");
  
  // Liste des conditions dans while :
  // STOP
  // AVANT
  // ARRIERE
  // IMPULSION
  // ACCELERE
  // DECELERE
  // VIRAGE A GAUCHE
  // VIRAGE A DROITE
  // TOUT DROIT
  // TOUPIE A GAUCHE
  // TOUPIE A DROITE
  // RAZ VITESSE ET VIRAGE
  // ATTENDRE
  // ARRET
  // REDEMARE
  // LINE FEED
  
  while(Serial.available()){
    //if (Serial.available() > 0) (crochet du =) //fonctionne à l_identique de while
        
    caractere=Serial.read(); // Lit le 1er octet reçu et le met dans la variable  
    Serial.println("  ");
    Serial.print ("caractere recu pour analyse avant conditions :  ");
    Serial.println (caractere);
    
    //---------------------------- A R R E T  : S T O P -------
    if (caractere==83 or caractere==115 or caractere==48) { //(83 = S, 115 = s, 48 ="0")
      arret();
      break;
    }
    
    //---------------------  A V A N T ---------------------
    if (caractere==65 or caractere==97 or caractere==56) { //(65 = A, 97 = a, 56 = "8")
      avant();
      break;
    }
    
    //--------------------------  A R R I E R E  -------------
    if (caractere==82 or caractere==114 or caractere==50) { //(82 = R, 114 = r, 50 = "2")
      arriere() ;
      break;
    }

    //--------------------------  I M P U L S I O N   -------------
    if (vit_nom < 51 and caractere==46) { //(46 = "." dot)
      // NOTA : les valeurs en cours sont conservées tels que vit_nom, sens* et r*.
      impulsion();
      delay(5);
      arret();
      break;
    }

    
    //-------------------------- A C C E L E R E -------------------------------------
    if (caractere==80 or caractere==112 or caractere==43 or caractere==57) { //(80 = P, 112 = p, 43 ="+", 57 = "9")
      //                          Afin d etre plus fin    Prévoir P = 25, p = 5, "+" = 1
      accelere();
      break;
    }
    
    //--------------------------  D E C E L E R E  --------------------------------------
    if (caractere==77 or caractere==109 or caractere==45 or caractere==55) { //(77 = M, 109 = m, 45 = "-", 55 = "7")
      decelere();
      break;
    }
    
    //--------------------------  V I R A G E   A   G A U C H E  ------------------------
    if (caractere==60 or caractere==52) { //(60 = "<", 52 = "4")
      virage_a_gauche();
      break;
    }
    
    //--------------------------  V I R A G E   A   D R O I T E  ------------------------
    if (caractere==62 or caractere==54) { //(62 = ">", 54 = "6")
      virage_a_droite();
      break;
    }
    
    //--------------------------  T O U T  D R O I T   ------------------------
    if (caractere==68 or caractere==100 or caractere==53) { //(68 = D, 100 = d, 53 = "5") 
      tout_droit();
      break;
    }
    
    //--------------------------  T O U P I E  A  G A U C H E    ------------------------
    if (caractere==116 or caractere==49) { //, 100 = t, 49 = "1") 
      toupie_a_gauche();
      break;
    }
    
    //--------------------------  T O U P I E  A  D R O I T E   ------------------------
    if (caractere==84 or caractere==51) { //(84 = T, 51 = "3") 
      toupie_a_droite();
      break;
    }

    //--------------------------  R A Z  V I T E S S E  E T V I R A G E   ------------------------
    if (caractere==42) { //(42 = "*") 
      raz_vitesse_virage();
      break;
    }
      
    //--------------  A T T E N D R E  A V A N T   N O U V E L L E   C D E    -----------
    if (caractere==119 or caractere==87 or caractere==47) { //(119 = W, 87 = w, 47 = "/") 
      //attendre();
      Serial.println("    Attendre x_milisecondes ");
      delay(x_milisecondes);
      break;
    } // fin Attendre avant nouvelle cde
    
    //--------------------------  S P A C E  B A R E    ------------------------
    //----------------------  A R R E T - R E D E M A R E     ------------------------
    if (caractere==32) { //(32 = SPACE BARE) 
     
      if (arret_moment == 1) {
        Serial.print("arret_moment  = ");
        Serial.print(arret_moment);
        Serial.println("   ARRET MEMENTANNE");
        arret_moment = 0;
        arret();
        
        delay (10);
      }
      //break;
     
     else {
       Serial.print("arret_moment  = ");
       Serial.print(arret_moment);
       Serial.println("   REDEMARE");
       arret_moment = 1;
        redemare();
        
        delay(10);
      }
      //break;
    } // fin Arret - Redemare
   
    //-----------------------------  L I N E   F E E D  -------------------
    if (caractere==10) {
      Serial.println ("   Line Feed");
      break;
    }
    
    
    delay (1);
  } // fin de while
  
  t = 1000;
  t2 = 2500;
      
}   // fin boucle programme, fin de la fonction loop() 

//******************************************************************
//__________________________________________________________________
//
//****************************  V O I D'  S ************************
//Liste des voids :

//void des actions par les touches pressées:
//void arret
//void avant
//void arriere
//void impulsion
//void accelere
//void decelere
//void virage_a_gauche
//void virage_a_droite
//void tout_droit
//void toupie_a_gauche
//void toupie_a_droite
//void raz_vitesse_virage

//void des fonctions de commandes du RASPIBOT
//void ralenti
//void redemare
//void calcul_vitesses
//void commande_moteurs

//**********************   void A R R E T      **********************
void arret() {
      Serial.println ("   ARRET");
      ralenti();
      /*
      vit_nomM = vit_nom;
      rGM = rG;
      rDM = rD;
      vit_nom = 0;
      calcul_vitesses ();
      commande_moteurs();
      */
      
      swledAVG = 0;
      swledARG = 0;
      swledAVD = 0;
      swledARD = 0;
      
      digitalWrite (ledAVG, swledAVG); //éteind les leds adequates
      digitalWrite (ledARG, swledARG);
      digitalWrite (ledAVD, swledAVD);
      digitalWrite (ledARD, swledARD);
}

//**********************   void A V A N T     **********************
void avant() {
        Serial.println ("   Avant");
      if (sens_G == 1 or sens_D == 1) {
        vit_nomM = vit_nom;
        vit_nom =0;
        vit_G = 0;
        vit_D = 0;
        commande_moteurs();
        delay (200);
        vit_nom = vit_nomM;
      }
      sens_G = 0;
      sens_D = 0;
      
      delta = int (vit_nomM/10);
      Serial.print (delta);
      vit_nom = 0;
      for (i=1; i<=10; i++) {
        vit_nom = vit_nom + delta;
        calcul_vitesses ();
        commande_moteurs();
        delay (10);
      }
}

//**********************   void A R R I E R E      **********************
void arriere() {
      Serial.println ("   Arriere");
      if (sens_G == 0 or sens_D == 0) {
        vit_nomM = vit_nom;
        vit_nom =0;
        vit_G = 0;
        vit_D = 0;
        commande_moteurs();
        delay (200);
        vit_nom = vit_nomM;
      }
      sens_G = 1;
      sens_D = 1;
      
      delta = int (vit_nomM/10);
      Serial.print (delta);
      vit_nom = 0;
      for (i=1; i<=10; i++) {
        vit_nom = vit_nom + delta;
        calcul_vitesses ();
        commande_moteurs();
        delay (10);
      }
}

//**********************    void I M P U L S I O N    **********************
void impulsion() {
      Serial.println ("   Impulsion");
      vit_nomM = vit_nom;
      vit_nom =50;    // voir si 50 est suffisant pour se rapprocher délicatement d un objet !
      calcul_vitesses ();
      commande_moteurs();
      //delay (200);    // voir si le temps est correcte avec la vitesse de 50 !
      delay (100);    // voir si le temps est correcte avec la vitesse de 50 !
      
      vit_nom = vit_nomM; // retour à l'origine
      calcul_vitesses ();
      commande_moteurs();
      delay (10);
}

//**********************   void A C C E L E R E     **********************
void accelere() {
      Serial.println ("   Plus vite");
      
      acc = g_plus;
      vit_nom = vit_nom + acc;
      if (vit_nom > 255) {
        vit_nom = 255;
      }
      calcul_vitesses ();
      commande_moteurs();
}

//**********************    void D E C E L E R E    **********************
void decelere() {
      Serial.println ("   Moins vite");
      dec = g_moins;
      vit_nom = vit_nom - dec;
      if (vit_nom < 0) {
        vit_nom = 0;
      }
      calcul_vitesses ();
      commande_moteurs();
}

//**********************     void V I R A G E   A   G A U C H E   **********************
void virage_a_gauche() {
      Serial.println ("   Vire Gauche");
      
      rG = rG * 0.9;
      rD = rD * 1.1;
      
      calcul_vitesses ();
      commande_moteurs();
}

//**********************     void V I R A G E   A   D R O I T E    **********************
void virage_a_droite() {
      Serial.println ("   Vire Droite");
      
      rG = rG * 1.1;
      rD = rD * 0.9;
      
      calcul_vitesses ();
      commande_moteurs();
}

//**********************     void T O U T  D R O I T   **********************
void tout_droit() {
      Serial.println ("   Tout Droit");
      ralenti ();
      rG = 1;
      rD = 1;
      redemare ();
}

//**********************    void T O U P I E  A  G A U C H E    **********************
void toupie_a_gauche() {
      Serial.println ("   Toupie a Gauche");
      ralenti ();
      rG = 1;
      rD = 1;
      sens_G = 1;
      sens_D = 0;
      redemare ();
}

//**********************     void T O U P I E  A  D R O I T E   **********************
void toupie_a_droite() {
      Serial.println ("   Toupie a Droite");
      ralenti ();
      rG = 1;
      rD = 1;
      sens_G = 0;
      sens_D = 1;
      redemare ();
}

//**********************     void R A Z  V I T E S S E  E T V I R A G E    ************
void raz_vitesse_virage() {
      Serial.println ("   RAZ Vitesse et Virage");
      ralenti ();
      vit_nomM = 0;
      rG = 1;
      rD = 1;
      sens_G = 0;
      sens_D = 0;
      redemare ();
}

//************************************************************************************
//                           void des fonctions de commandes du RASPIBOT
//************************************************************************************

//**********************   R A L E N T I    **********************
void ralenti() {
        vit_nomM = vit_nom;
        vit_nom =0;
        vit_G = 0;
        vit_D = 0;
        commande_moteurs();
        delay (200);
        vit_nom = vit_nomM;
}

//*********************   R E D E M A R R E    ********************
void redemare() {
  
      delta = int (vit_nomM/10);
      Serial.print (delta);
      vit_nom = 0;
      for (i=1; i<=10; i++) {
        vit_nom = vit_nom + delta;
        calcul_vitesses ();
        commande_moteurs();
        delay (10);
      }
}
//****************   C A L C U L S   V I T E S S E S   *************

void calcul_vitesses() {
      vit_G = int (vit_nom * rG);
      if (vit_G > 255) {
        vit_G = 255;
      }
      vit_D = int (vit_nom * rD);
      if (vit_D > 255) {
        vit_D = 255;
      }
      Serial.print("vit_nom = ");
      Serial.print(vit_nom);
      Serial.print(", ");
      Serial.print("rG = "); 
      Serial.print(rG);
      Serial.print(", rD = "); 
      Serial.print(rD);
      Serial.print(", ");
}

//******************************************************************
void commande_moteurs() {

   //switch led AVG
  if (sens_G == 0){ 
    swledAVG = 1;
    swledARG = 0;
  }
  else
  {
    swledAVG = 0;
    swledARG = 1;
  }
  
  //switch led AVD
  if (sens_D == 0){ 
    swledAVD = 1;
    swledARD = 0;
  }
  else
  {
    swledAVD = 0;
    swledARD = 1;
  }
  
  digitalWrite (M1, sens_G);  // défini le sens de rotation moteur 1
  digitalWrite (M2, sens_D);  // défini le sens de rotation moteur 2
  digitalWrite (ledAVG, swledAVG); //allume les leds adequates
  digitalWrite (ledARG, swledARG);
  digitalWrite (ledAVD, swledAVD);
  digitalWrite (ledARD, swledARD);
  
  analogWrite (E1, vit_G); //controle vitesse PWM moteur 1
  analogWrite (E2, vit_D); //controle vitesse PWM moteur 2
  
  Serial.print("Moteur G, ");
  if (sens_G ==0) {
    Serial.print("Avant,  ");
    Serial.print("vit_G =");
    Serial.print (vit_G);
    
  }
  else {
    Serial.print("Arriere, ");
    Serial.print("vit_G =");
    Serial.print (vit_G);
  }
  
   Serial.print(";    Moteur D, ");
  if (sens_D ==0) {
    Serial.print("Avant,  ");
    Serial.print("vit_D =");
    Serial.println (vit_D);
  }
  else {
    Serial.print("Arriere, ");
    Serial.print("vit_D =");
    Serial.println (vit_D);
  }
  
}//fin void commande_moteurs()

//********************* F I N   D U   P R O G R A M M E   ******************

