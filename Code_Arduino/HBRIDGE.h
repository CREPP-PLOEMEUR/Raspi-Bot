/* Bibliothèque HBRIDGE pour commande moteur cc by Nicolas LE GUERROUE
 * Version : 20.03.17
 */
#include "Arduino.h"

#define MAX 255
#define MIN 0
#define MED 127

class HBRIDGE 		//DEBUT CLASSE HBRIDGE
{
public:

   HBRIDGE(byte enable_voie_gauche, byte sens_voie_gauche, byte enable_voie_droite, byte sens_voie_droite);
   void debug();//avance, recule, va à droite puis à gauche en 4 s

   void motor_on(byte sens_voie_gauche, byte val_voie_gauche, byte sens_voie_droite, byte val_voie_droite);
   void motor_on(byte sens_voie_gauche, byte val_voie_gauche, unsigned long delai_motor_gauche_on,     byte sens_voie_droite, byte val_voie_droite, unsigned long delai_motor_droite_on);
   void motor_prog(byte sens_voie_gauche, byte val_voie_gauche, int delai_acceleration_voie_gauche,      byte sens_voie_droite, byte val_voie_droite, int delai_acceleration_voie_droite);
   void motor_off(bool state_voie_gauche, bool state_voie_droite);


private:

	byte _enable_voie_gauche; 
	byte _sens_voie_gauche;
	byte _enable_voie_droite;
	byte _sens_voie_droite;

   byte val_voie_gauche = 100; //valeur par défaut
   byte val_voie_droite = 100;

   int delai_pas;
   unsigned long delta_delai;

};



