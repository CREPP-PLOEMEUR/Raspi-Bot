#include <SoftwareSerial.h>


SoftwareSerial mavoieserie(15, 14);// Rx Tx

int caractere;

void setup() {

    Serial.begin(115200); 
    mavoieserie.begin(9600);//35400 en mode programmation sinon 9600 par defaut  

}

void loop() {
  
 //D'arduino vers Rpi (taper dans monit serie Arduino, vit 115200, NL + CR)
      //while (Serial.available() > 0) {
           // caractere = Serial.read();
           // Serial.println("ok");
           // mavoieserie.write(caractere);
             
//de Rpi vers Arduino (taper dans moniteur serie de Rpi (vit 9600 bauds, NL + CR)
      while (mavoieserie.available() > 0) {
           caractere = mavoieserie.read();   
            Serial.println (caractere);
    

      }     
      }
