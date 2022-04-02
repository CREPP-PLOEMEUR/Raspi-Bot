//BIBLIOTHEQUE UART 
#include "Arduino.h"
#include "UART.h"
#include "SoftwareSerial.h"

UART::UART(int rx, int tx)
   : _serial(rx, tx)
{
	
}

void UART::begin(unsigned long bauds) {
	_serial.begin(bauds);
}
String UART::read() {

 String message="";

if(_serial.available()>0) {
 while(_serial.available()>0) {

	c = _serial.read();
	message = message + c;
	delay(2);
	}//fin while

return(message);
}

}//fin lecture


String UART::read(uint8_t position) {

String message="";

if(position==LOCKED) {

 do {delayMicroseconds(10);}
 while(_serial.available()==0); 
 
  while(_serial.available()>0)
  
  {
	c = _serial.read();
	message = message + c;
	delay(2);
	}//fin while

return(message);

}//fin do while
}//fin lecture

void UART::send(String message) {

 	_serial.println(message);
}//fin send

void UART::send(int message) {

 	_serial.println(message);
}//fin send

//FIN UART