/* Bibliothèque HBRIDGE pour commande moteur cc by Nicolas LE GUERROUE
 * Version : 20.03.17
 */
#include "Arduino.h"
#include "HBRIDGE.h"



HBRIDGE::HBRIDGE(byte enable_voie_gauche, byte sens_voie_gauche, byte enable_voie_droite, byte sens_voie_droite)  
{
	
	_enable_voie_gauche = enable_voie_gauche;
	_sens_voie_gauche = sens_voie_gauche;
	_enable_voie_droite = enable_voie_droite;
	_sens_voie_droite = sens_voie_droite;
	
	pinMode(_enable_voie_gauche, OUTPUT);
	pinMode(_enable_voie_droite, OUTPUT);
	pinMode(_sens_voie_gauche, OUTPUT);
	pinMode(_sens_voie_droite, OUTPUT);
}


void HBRIDGE::debug() {

	pinMode(_enable_voie_gauche, OUTPUT);
	pinMode(_enable_voie_droite, OUTPUT);
	pinMode(_sens_voie_gauche, OUTPUT);
	pinMode(_sens_voie_droite, OUTPUT);

	
}


void HBRIDGE::motor_on(byte sens_voie_gauche, byte val_voie_gauche, byte sens_voie_droite, byte val_voie_droite) {

	analogWrite(_enable_voie_gauche, val_voie_gauche);
	analogWrite(_enable_voie_droite, val_voie_droite);
	digitalWrite(_sens_voie_gauche, sens_voie_gauche);
	digitalWrite(_sens_voie_droite, sens_voie_droite);


}

void HBRIDGE::motor_on(byte sens_voie_gauche, byte val_voie_gauche, unsigned long delai_motor_gauche_on,     byte sens_voie_droite, byte val_voie_droite, unsigned long delai_motor_droite_on ) {

	delta_delai = delai_motor_gauche_on - delai_motor_droite_on;

	if(delta_delai>0) {//si delta>0, alors delai_gauche>delai_droite

		analogWrite(_enable_voie_gauche, val_voie_gauche);
		analogWrite(_enable_voie_droite, val_voie_droite);
		digitalWrite(_sens_voie_gauche, sens_voie_gauche);
		digitalWrite(_sens_voie_droite, sens_voie_droite);
		delay(delai_motor_droite_on);
		motor_off(1,0);//coupe la voie droite
		delay(delta_delai);
		motor_off(0,0); //coupe toutes les voies
	}
	
	if(delta_delai<0) { //si delta<0, alors delai_gauche<delai_droite

		analogWrite(_enable_voie_gauche, val_voie_gauche);
		analogWrite(_enable_voie_droite, val_voie_droite);
		digitalWrite(_sens_voie_gauche, sens_voie_gauche);
		digitalWrite(_sens_voie_droite, sens_voie_droite);
		delay(delai_motor_gauche_on);
		motor_off(0,1);//coupe la voie gauche
		//delay(abs(delta_delai));
		motor_off(0,0); //coupe toutes les voies

	}

	if(delta_delai==0) { //les moteurs se coupent en même temps

		analogWrite(_enable_voie_gauche, val_voie_gauche);
		analogWrite(_enable_voie_droite, val_voie_droite);
		digitalWrite(_sens_voie_gauche, sens_voie_gauche);
		digitalWrite(_sens_voie_droite, sens_voie_droite);
		//delay(delai_motor_gauche_on);//attend le delai d'un moteur (gauche ou droit est similaire !!)
		motor_off(0,0); //coupe toutes les voies
	}

}

void motor_prog(byte sens_voie_gauche, byte val_voie_gauche, int delai_acceleration_voie_gauche,      byte sens_voie_droite, byte val_voie_droite, int delai_acceleration_voie_droite) {



}

void HBRIDGE::motor_off(bool state_voie_gauche, bool state_voie_droite) {

	if(state_voie_gauche==false) {
	analogWrite(_enable_voie_gauche, 0);
	digitalWrite(_sens_voie_gauche, 0);
	}

	if(state_voie_droite==false) {
	analogWrite(_enable_voie_droite, 0);
	digitalWrite(_sens_voie_droite, 0);	
	}
}//fin arret
