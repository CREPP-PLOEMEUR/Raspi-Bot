// Bibliothèque DISPLAY pour affichage 7 segments by Nicolas LE GUERROUE

#include "Arduino.h"
#include "LED.h"

LED::LED(byte led) {

	_led = led;	
	pinMode(_led, OUTPUT);
	
}//fin constructeur


void LED::on() {digitalWrite(_led, 1);}

void LED::on(unsigned long delai) {

	digitalWrite(_led, 1);
	delay(delai);
	digitalWrite(_led, 0);
}//fin on



void LED::pwm(byte valeur) {analogWrite(_led, valeur);}

void LED::pwm(byte valeur, unsigned long delai) {

	analogWrite(_led, valeur);
	delay(delai);
	analogWrite(_led, 0);
}//fin pwm

void LED::blink(unsigned long delai_haut, unsigned long delai_bas, unsigned long nombre_de_fois) {

	for(int i=0;i<nombre_de_fois;i++) {

		digitalWrite(_led, 1);
		delay(delai_haut);
		digitalWrite(_led, 0);
		delay(delai_bas);
	}//fin for

}//fin blink

void LED::off() {digitalWrite(_led, 0);}