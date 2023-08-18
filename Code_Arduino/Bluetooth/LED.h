// Bibliothèque LED pour led by Nicolas LE GUERROUE

#include "Arduino.h"
#define DEFAUT 13

class LED
{
public:
	
	LED(byte led);

	void on();//allume la led
	void on(unsigned long delai); //allume la led pendant un delai
	void pwm(byte valeur);//allume la led avec une broche PWM
	void pwm(byte valeur, unsigned long delai);//allume la led avec une broche PWM pendant un delai
	void blink(unsigned long delai_haut, unsigned long delai_bas, unsigned long nombre_de_fois);//fait clignoter la led
	void off();//eteint la led
	

	byte _led;

};