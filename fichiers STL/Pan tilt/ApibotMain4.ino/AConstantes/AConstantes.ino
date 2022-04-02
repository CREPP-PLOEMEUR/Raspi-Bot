//**CONSTANTES GENERALES**

//**CONSTANTES COMMUNICATION **

int nombreReception = NULL;//nombre (parametre) en reception
long params[10] ;//tableau des parametres sous la forme :
//[sensMotG,sensMotD,vitMotG,vitMotD,RotationCam,ElevationCam,RotationBras,ElevationBras,
//AvArBras,PinceBras]
int caractere;// caractere unitaire recu sur le port serie
int i =0;//compteur de parametres recus
int stock;//detection des virgules(2 a suivre ou pas)


//**CONSTANTES MOTEURS**

int sensG =0;// variable sens moteur gauche
int sensD= 0;// variable sens moteur droit
int vitG =0; // variable vitesse moteur gauche
int vitD =0; // variable vitesse moteur Droit


    //  Déclaration des broches PWM moteur(analogique)
    const int VITESSE_G=11; // broche vitesse moteur gauche (E1)
    const int VITESSE_D=9; // broche vitesse moteur droit (E2)
    
    //  Déclaration des broches moteur numeriques
    const int SENS_MOTG=10; // broche sens rotation moteur gauche (M1)
    const int SENS_MOTD=8; // broche sens rotation moteur droit (M2)
    

//**CONSTANTES CAMERA**

int CamHB =60; //variable angle Camera haut/bas
int CamRot =60; // variable angle Camera rotation
int vitesseCam=50;//vitesse de positionnement des servos

// positions extremes des servos
const int MinHB=550; // largeur impulsion en µs correspondant à la position min du servo camHB
const int MaxHB=2250; // largeur impulsion en µs correspondant à la position max du servo camHB
const int MinRot=550; // largeur impulsion en µs correspondant à la position min du servo camRot
const int MaxRot=2250; // largeur impulsion en µs correspondant à la position max du servo camRot


    //Déclaration des broches 
    const int CameraHB=2; // broche servo camera haut/bas
    const int CameraRot=3;//broche servo camera rotation
    
    //declaration d'objets servos
    Servo ScamHB;
    Servo ScamRot;

//**CONSTANTES BRAS**

int BrasPince = 0;// variable pince
int BrasAvAr = 0; // variable bras avant/arriere
int BrasElev = 0; // variable bras elevation
int BrasRot = 0; // variable bras rotation
int vitesseBras=50;//vitesse de positionnement des servos

    //Déclaration des broches
    const int BPince=8;//Broche servo pince
    const int BAvAr =9;//Broche servo bras Avant/arriere
    const int BElev =10;//Broche servo bras elevation
    const int BRot = 11; //Broche servo bras rotation

int valeurs[10] = {sensG,vitG,sensD,vitD,CamHB,CamRot,BrasPince,BrasAvAr,BrasElev,BrasRot};
// tableau regroupant les differentes valeurs.




// Declaration constantes diverses







