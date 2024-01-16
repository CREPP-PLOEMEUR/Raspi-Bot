// Bibliothèque UART pour module bluetooth type Crius ou HC-05 
// AUTEUR : Nicolas LE GUERROUE

#include "Arduino.h"
#include "SoftwareSerial.h"

 #define CRIUS 115200
 #define HC_05 9600
 #define LOCKED 0

class UART 		//DEBUT CLASSE UART
{
public:
   	UART(int rx, int tx);  //rx de la carte arduino
   	String read(void);//lit si présence de donnée
   	String read(uint8_t position);//si arg est LOCKED, ne quitte pas tant qu'il n'y a pas de donnée
    void begin(unsigned long bauds);
   	void send(String message);//envoie la donnée en arg
   	void send(int message);//envoie la donnée en arg
private:
  	SoftwareSerial _serial;
  	
   	char c;
   	unsigned long _bauds;

}; //FIN CLASSE UART


