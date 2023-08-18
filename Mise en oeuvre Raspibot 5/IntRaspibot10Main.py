#!/usr/bin/python
# -*- coding: utf-8 -*-

# modules a importer 
from PyQt4.QtGui import *
from PyQt4.QtCore import *  
import glob
import os,sys
import serial  # bibliothèque permettant la communication série
import time    # pour le délai d'attente entre les messages

from IntRaspibotDesign10 import * 
memclicVG=40
memclicVD=40
VG=0
VD=0
sensG = 0
sensD = 0

sensibilite = 4 #(augmenter pour diminuer la sensibilite du pad moteurs)

class myApp(QWidget, Ui_InterfaceCommande): # la classe reçoit le Qwidget principal ET la classe définie dans test.py obtenu avec pyuic4
	def __init__(self, parent=None):
		
		QWidget.__init__(self) # initialise le Qwidget principal 
		self.setupUi(parent) # Obligatoire 
		
		#Signaux (servo)moteurs
		self.connect(self.RBS, SIGNAL("sliderReleased()"), self.RBSReleased) 
		self.connect(self.EBS, SIGNAL("sliderReleased()"), self.EBSReleased)
		self.connect(self.AvArBS, SIGNAL("sliderReleased()"), self.AvArBSReleased)
		self.connect(self.PinceS, SIGNAL("sliderReleased()"), self.PinceSReleased)
		self.connect(self.RCS, SIGNAL("sliderReleased()"), self.RCSReleased)
		self.connect(self.HBCS, SIGNAL("sliderReleased()"), self.HBCSReleased)
		self.connect(self.RAZgeneral, SIGNAL("clicked()"), self.RAZgeneralClicked)
		self.connect(self.MGini, SIGNAL("clicked()"), self.MGiniClicked)
		self.connect(self.MDini, SIGNAL("clicked()"), self.MDiniClicked)
		self.connect(self.Stopper, SIGNAL("clicked()"), self.StopperClicked)
		self.connect(self.RotCini, SIGNAL("clicked()"), self.RotCiniClicked)
		self.connect(self.HBCini, SIGNAL("clicked()"), self.HBCiniClicked)
		self.connect(self.RBini, SIGNAL("clicked()"), self.RBiniClicked)
		self.connect(self.ElevBini, SIGNAL("clicked()"), self.ElevBiniClicked)
		self.connect(self.AvArBini, SIGNAL("clicked()"), self.AvArBiniClicked)
		self.connect(self.pinceini, SIGNAL("clicked()"), self.pinceiniClicked)
		#Signaux port serie
		self.serialPort=None
		self.connect(self.Envoi, SIGNAL("clicked()"), self.EnvoiClicked)
		self.connect(self.Connexion, SIGNAL("clicked()"), self.ConnexionClicked)
		#initialisation Timer
		self.timer=QTimer()
		self.connect(self.timer,SIGNAL("timeout()"), self.timerEvent)
		#declaration filtre d'evenements
		self.moteurs.installEventFilter(self)
		#self.connect(self.moteurs, SIGNAL("clicked()"),self.clicmoteurClicked)
		
		# DEF CONSOLE SERIE
		
	def PortDetect(self):#detection des ports serie disponibles			
		if sys.platform.startswith('linux') :
	        # exclusion terminal courant
			ports = glob.glob('/dev/tty[A-Za-z]*')
		else:
			raise EnvironmentError('Unsupported platform')
		result = []
		i=0
		for port in ports:
			try:			
				s = serial.Serial(port)
				s.close()
				result.append(port)
				self.PortSerie.insertItem(i,port,0)
				i = i+1					
			except (OSError, serial.SerialException):
				pass
		return result	

		
	def EnvoiClicked(self):#envoi manuel d'instructions
		if self.serialPort:
			self.serialPort.write(str(self.lineEditChaineEnvoi.text())+ '\n' )# envoie le texte tape a aérduino
			commande =[0,1,2,3,self.HBCS,self.RCS,self.RBS,self.AvArBS,self.EBS,self.PinceS]
			chaine = self.lineEditChaineEnvoi.text()
			indice = int(chaine[0])# indice envoye
			mvmt = int(chaine[1:])# valeur envoyee
			if (indice>3):
				commande[indice].setValue(mvmt)
			if (indice==0):
				VG = mvmt
				memclicVG = mvmt
				self.sourisY.display(VG)
			if (indice==1):
				VD = mvmt
				memclicVD = mvmt
				self.sourisX.display(VD)				
			if (indice==2):
				sensG = mvmt
			if (indice==3):
				sensD = mvmt

			self.lineEditChaineEnvoi.setText("")
			

	def ConnexionClicked(self): #  bouton connexion				               
			if  self.PortSerie.currentText()=="" : # si le champ  Port est vide = detection
				self.PortDetect() 
			else :
				strPortInit=str(self.PortSerie.currentText()) #sinon utilise paramètre selectionne pour le port
				strDebitInit=str(self.Vcom.currentText()) # paramètre selectionne pour debit
                     
			try: 
                        # initialise port serie avec délai attente en réception en sec
				self.serialPort=serial.Serial(strPortInit, strDebitInit, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE, 0.100)                     
				self.serialPort.flushInput() # vide la file d'attente série
				self.Connexion.setStyleSheet(QString.fromUtf8("background-color: rgb(0, 255, 0);")) # bouton en vert(connexion OK)
				self.Connexion.setText("       OK")  # change titre bouton
					
			except: # si erreur initialisation                                 
				self.Connexion.setStyleSheet(QString.fromUtf8("background-color: rgb(255, 127, 0);")) # bouton en orange (non connecte)
				self.Connexion.setText(QString.fromUtf8("Non connecté"))  # change titre bouton

			self.timer.start(10) # lance le timer avec délai en ms - 10 pour réception rapide 
                
	                
				
	def timerEvent(self): # fonction appelée lors de la survenue d'un évènement Timer - nom fonction indiférrent
			self.chaineIn="";
			self.char="";
                # lecture des données reçues           
			if self.serialPort: # seulement si le port série existe
					self.timer.stop() # stoppe le timer le temps de lire les caractères et éviter "réentrée"
					
					while (self.serialPort.inWaiting()): # tant que au moins un caractère en réception
							self.char=self.serialPort.read() # on lit le caractère
							if self.char=='\n': # si saut de ligne, on sort du while
									#print("saut ligne reçu") # debug
									break # sort du while
							else: #tant que c'est pas le saut de ligne, on l'ajoute à la chaine
									self.chaineIn=self.chaineIn+self.char                                                                

					if len(self.chaineIn)>0: # ... pour ne pas avoir d'affichage si ""     
							print(self.chaineIn) # affiche la chaîne
							#self.textReception.append(self.chaineIn[:-1]) # ajoute le texte - le dernier caractère
							self.textReception.append(self.chaineIn) # ajoute le texte - le dernier caractère

					self.timer.start() # redémarre le timer
					
	#def clicmoteurClicked(self):
		#self.clic.setGeometry(QtCore.QRect(Xc,Yc,3,3))				
			#DEF PAD MOTEURS
		
	def eventFilter(self, _, event): # fonction pour gérer filtrage évènements		
		if event.type() == QEvent.MouseButtonPress:
			
			Xc=event.pos().x() # coordonnée souris 
			Yc=event.pos().y()
			X=Xc - 127
			Y = 127 - Yc
			global VG 
			global VD 
			global memclicVG
			global memclicVD
			global sensG
			global sensD
			global sensibilite
			
			#for colX in range (Xc-2,Xc+2):
				#for colY in range (Yc-2,Yc+2):
			#self.widget.move(QString.fromUtf8("selection-color: rgb(255, 0, 0);"))
			#self.clic.move(QtGui.QWidget(self.clic.move(Xc,Yc))
			#self.position = moteurs(self.clic)
			#self.position.move(Xc,Yc)
			self.clic.setGeometry(QtCore.QRect(Xc,Yc,3,3))				

			
			if Y>0:
				sensG=0
				sensD=0
			else:
				sensG=1
				sensD=1				
			
			if ((Y>109) or (Y<-119)):
				if (X<-13):					
					VG = 255
					VD = memclicVD
					
				elif (X>13):
					VD = 255
					VG= memclicVG
				else:
					VG = 255
					VD = 255
						
			if ((Y>-21) and (Y<11)):								
					VG= 40
					VD= 40
					memclicVD = VD
					memclicVG = VG
					
			if ((Y>10) and (Y<110)):				
				if  (X<-13):
					VGi,p = str(18.5+2.15*Y).split(".")
					VG = int(VGi)
					a = abs(memclicVG-VG)
					if (a<sensibilite):
						VG = memclicVG
					VDi,q = str((VG-40)*X/114 +1.11*VG - 2.3).split(".")
					VD = int(VDi)
					memclicVG=VG
					memclicVD = VD #					
					if self.InversionB.isChecked() :
						sensD = 1- sensD					
					
				elif (X>13):
					VDi,p = str(18.5+2.15*Y).split(".")
					VD= int(VDi)
					a = abs(memclicVD-VD)
					if (a<sensibilite):
						VD=memclicVD
					VGi,q = str((VD-40)*X/-114 +1.11*VD-2.3).split(".")
					VG = int(VGi)
					memclicVD = VD
					memclicVG = VG#
					if self.InversionB.isChecked() :
						sensG = 1 - sensG						
				else:
					VGi,p = str(18.5+2.15*Y).split(".")
					VG = int(VGi)
					VD = VG	
					memclicVG = VG#
					memclicVD = VD#
					
			if ((Y<-20) and (Y>-120)):				
				if  (X<-13):
					VGi,p = str(-3-2.15*Y).split(".")
					VG= int(VGi)
					a = abs(memclicVG-VG)
					if (a<sensibilite):
						VG = memclicVG
					VDi,q = str((-VG+40)*X/-114 +1.11*VG - 2.3).split(".")
					VD= int(VDi)
					memclicVG = VG
					memclicVD = VD#
					if self.InversionB.isChecked() :
						sensD = 1 - sensD

				elif (X>13) :
					VDi,p = str(-3-2.15*Y).split(".")
					VD= int(VDi)
					a = abs(memclicVD-VD)
					if (a<sensibilite):
						VD = memclicVD
					VGi,q = str((VD-40)*X/-114 +1.11*VD - 2.3).split(".")	
					VG = int(VGi)
					memclicVD = VD
					memclicVG = VG#		
					if self.InversionB.isChecked():
						sensG = 1 - sensG
				else:
					VGi,p = str(-3-2.15*(Y)).split(".")
					VG = int(VGi)
					VD = VG	
					memclicVG = VG#
					memclicVD = VD#	

			self.sourisX.display(VD)
			self.sourisY.display(VG)			
			#print sensG
			#print sensD
			#print VG
			#print VD
			
			SD = str(3) + str(sensD) + "\n"
			SG = str(2) + str(sensG) + "\n"
			MD = str(1) + str(VD) + "\n"
			MG = str(0) + str(VG) + "\n"
			self.serialPort.write(SD)
			time.sleep(0.001)
			self.serialPort.write(SG)
			time.sleep(0.001)
			self.serialPort.write(MD)
			time.sleep(0.001)
			self.serialPort.write(MG)


		return False # obligatoire...
		
	def StopperClicked(self):
		global sensG
		global sensD
		global VD
		global VG
		VD=40
		VG=40
		self.sourisX.display(VD)#valeur d'arret moteur gauche
		self.sourisY.display(VG)#valeur d'arret moteur droit
		SD = str(3) + str(sensD) + "\n"
		SG = str(2) + str(sensG) + "\n"
		MD = str(1) + str(VD) + "\n"
		MG = str(0) + str(VG) + "\n"  
		self.serialPort.write(SD)
		time.sleep(0.001)
		self.serialPort.write(SG)
		time.sleep(0.001)
		self.serialPort.write(MD)
		time.sleep(0.001)
		self.serialPort.write(MG)
				
	def MGiniClicked(self):
		global sensG
		global sensD
		global VD
		global VG		
		self.sourisY.display(40)#valeur d'arret moteur droit
		VG = 40
		SD = str(3) + str(sensD) + "\n"
		SG = str(2) + str(sensG) + "\n"
		MD = str(1) + str(VD) + "\n"
		MG = str(0) + str(VG) + "\n"  
		self.serialPort.write(SD)
		time.sleep(0.001)
		self.serialPort.write(SG)
		time.sleep(0.001)
		self.serialPort.write(MD)
		time.sleep(0.001)
		self.serialPort.write(MG)
		
	def MDiniClicked(self):	
		global sensG
		global sensD
		global VD
		global VG
		self.sourisX.display(40)
		VD = 40
		SD = str(3) + str(sensD) + "\n"
		SG = str(2) + str(sensG) + "\n"
		MD = str(1) + str(VD) + "\n"
		MG = str(0) + str(VG) + "\n"
		self.serialPort.write(SD)
		time.sleep(0.001)
		self.serialPort.write(SG)
		time.sleep(0.001)
		self.serialPort.write(MD)
		time.sleep(0.001)
		self.serialPort.write(MG)
	
		
		#DEF BOUTONS INI
		
	def HBCiniClicked(self):# init haut/bas camera
		global mema
		self.HBCS.setValue(60)
		a = str(460) + "\n"
		mema = a
		self.serialPort.write(a)
		time.sleep(0.001)
		
	def RotCiniClicked(self):# init rotation camera
		global memb
		self.RCS.setValue(60)
		b = str(560) + "\n"
		memb = b
		self.serialPort.write(b)
		time.sleep(0.001)
		
	def RBiniClicked(self):# init rotation bras
		global memc
		self.RBS.setValue(60)
		c = str(660) + "\n"
		memc = c
		self.serialPort.write(c)
		time.sleep(0.001)
		
	def AvArBiniClicked(self):# init Av/Ar Bras
		global memd
		self.AvArBS.setValue(60)	
		d = str(760) + "\n"
		memd = d
		self.serialPort.write(d)
		time.sleep(0.001)		

		
	def ElevBiniClicked(self):# init elevation bras
		global meme
		self.EBS.setValue(60)
		e = str(860) + "\n"
		meme = e
		self.serialPort.write(e)
		time.sleep(0.001)			
		
	def pinceiniClicked(self): #init Pince
		global memf
		self.PinceS.setValue(60)
		f = str(960) + "\n"
		memf = f
		self.serialPort.write(f)
		time.sleep(0.001)		
		
	def RAZgeneralClicked(self):
		self.HBCiniClicked()
		self.RotCiniClicked()
		self.RBiniClicked()
		self.AvArBiniClicked()
		self.ElevBiniClicked()
		self.pinceiniClicked()
				
		
		#DEF SLIDERS
		
	def HBCSReleased(self): # Haut Bas Camera
		a = str(4) + str(self.HBCS.value()) + "\n" 		
		self.serialPort.write(a)
		
	def RCSReleased(self): # Rotation Camera
		b=str(5)+str(self.RCS.value()) + "\n" 
		self.serialPort.write(b)	
				
	def RBSReleased(self): # Rotation Bras
		global memc		
		c = str(6)+str(self.RBS.value()) + "\n" 
		self.serialPort.write(c)
		
	def AvArBSReleased(self): # Avant/Arriere Bras
		d=str(7)+str(self.AvArBS.value()) + "\n" 
		self.serialPort.write(d)
					
	def EBSReleased(self): # Elevation Bras
		e=str(8)+str(self.EBS.value()) + "\n"
		self.serialPort.write(e)
	
	def PinceSReleased(self): # Pince Bras
		f=str(9)+str(self.PinceS.value()) + "\n"
		self.serialPort.write(f)	
		        
        
def main(args):
	global a
	a=QApplication(args) # crée l'objet application 
	f=QWidget() # crée le QWidget racine
	c=myApp(f) # appelle la classe contenant le code de l'application 
	f.show() # affiche la fenêtre QWidget
	c.PortDetect()#routine de detection du port
	r=a.exec_() # lance l'exécution de l'application 
	return r
		

if __name__=="__main__": # pour rendre le code exécutable 
	main(sys.argv) # appelle la fonction main


