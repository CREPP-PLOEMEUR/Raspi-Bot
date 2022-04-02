#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  mainvideo.py
#  
#  Copyright 2017 thierry <thierry@levronpchp>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.  

# modules a importer
from PyQt4.QtGui import *
from PyQt4.QtCore import *  
import os,sys
import serial
from designZ7 import * # fichier obtenu à partir QtDesigner et pyuic4

comconcat = "raspivid"
stream = ""
s = 5

				
class myApp(QWidget, Ui_Form): # la classe reçoit le Qwidget principal ET la classe définie dans test.py obtenu avec pyuic4
		def __init__(self, parent=None):
			QWidget.__init__(self) # initialise le Qwidget principal
			self.setupUi(parent) # Obligatoire			
			
 #Réalisez les connexions supplémentaires entre signaux et slots
 # connecte le signal Clicked de l'objet bouton à l'appel de la fonction voulue
 			self.connect(self.VLC, SIGNAL("pressed()"), self.VLCclicked)
 			self.connect(self.GSTREAM, SIGNAL("pressed()"), self.GSTREAMclicked)
 			self.connect(self.RAZ, SIGNAL("clicked()"), self.RAZclicked)
 			self.connect(self.HDMI, SIGNAL("pressed()"), self.HDMIclicked)	
 			self.connect(self.Executer, SIGNAL("pressed()"), self.Executerclicked)

 			
		def concatenation(self):
							
			global comconcat							
			action =[self.Vstabilite.currentText(),self.Veffets.currentText(),self.Vhauteur.currentText(),self.VLargeur.currentText(),self.Vhflip.currentText(),self.Vvflip.currentText(),self.Vmode.currentText(),self.Vcompression.currentText(),self.Vmetadata.currentText(),self.Vpecran.currentText(),self.Viso.currentText(),self.Vrotation.currentText()]			
			commande = [" "," -ifx "," -h "," -w ","","","-md "," -drc "," -a "," "," -ISO "," -rot "]
			action2 = [self.Vframerate.value(),self.Vfinesse.value(),self.Vcontraste.value(),self.Vbrillance.value(),self.Vsaturation.value()]			
			commande2 = [" -fps "," -sh "," -co "," -br "," -sa "]
			
			if self.Vduree.currentText() <> "inactif":						
				duree= int(self.Vduree.currentText())						
				comconcat = str(comconcat + " -t " + str(duree*60000))
				
			if self.Vsauvegarde.text() <> "":
				sauvegarde1= "-o " + str(self.Vsauvegarde.text())
				
					
									
				#sauvegardeCCT= str(" -o " + str(sauvegarde1))
				comconcat = str(comconcat + sauvegarde1)
				print comconcat
									
			for item in action:
				if item <> "inactif":
					concat = commande[action.index(item)]																				
					comconcat = comconcat + concat + item 
										
			for item2 in action2:
				if item2 <> 0:
					concat2 = commande2[action2.index(item2)]																				
					comconcat = str( comconcat + concat2 + str(item2 ))														
			
		def RAZclicked(self):
			global comconcat
			comconcat ="raspivid"
			self.comfinale.setText(QString.fromUtf8(comconcat)	)
			
		def HDMIclicked(self):
			global s
			s = 1
			global comconcat
			global stream
			stream = str("")		
			self.concatenation()
			self.comfinale.setText(QString.fromUtf8(comconcat))

		def GSTREAMclicked(self):
			global s
			s = 2
			global comconcat
			global stream
			self.concatenation()
			concat2 = str(" -o -n ")
			stream = str(" | gst-launch -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.103 port= 5000 ")
			comconcat = comconcat + concat2
			self.comfinale.setText(QString.fromUtf8(comconcat)	)	
			
		def VLCclicked(self):
			global s
			s = 3
			global comconcat
			global stream						
			self.concatenation()
			concat2 = str(" -o -n ")
			stream = str("| cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264 ")
			comconcat = comconcat + concat2
			self.comfinale.setText(QString.fromUtf8(comconcat)	)
			
			
		def Executerclicked(self):
			global s
			global stream
			global comconcat
			commande = comconcat + stream
			bang = str("#!/bin/bash\n")
			ecriture = bang + commande
			print ecriture
			os.chdir("/home/thierry/Bureau/Videopi")
			
			if s == 1:
				try :
					with open ('hdmi.sh','w') as monfichier:
						monfichier.write(ecriture)
			#os.chmod("/home/thierry/Bureau/essai.sh", 0777)	
					a.quit()
					
				except IOError:
					print "Ecriture impossible dans le fichier hdmi.sh"
						
			if s == 3:
				try:
					with open ('vlcstream.sh','w') as monfichier:
						monfichier.write(ecriture)
					a.quit()
					
				except IOError:
					print "Ecriture impossible dans le fichier vlcstream.sh"
					
					
			if s == 2:
				try:
					with open ('gstream.sh','w') as monfichier:
						monfichier.write(ecriture)
					a.quit()
						
				except IOError :
					print "Ecriture impossible dans le fichier gstream.sh"

			

def main(args):
	global a
	a=QApplication(args) # crée l'objet application
	f=QWidget() # crée le QWidget racine
	c=myApp(f) # appelle la classe contenant le code de l'application
	f.show() # affiche la fenêtre QWidget
	r=a.exec_() # lance l'exécution de l'application
	return r	


if __name__ == '__main__':
	main(sys.argv)

