#!/bin/bash

port='5000'
# pour la ligne suivante,mettre l'ip la plus souvent utilisee
# ce sera l'ip par defaut
IP='192.168.0.103'
choix='5'


while [ $choix != '1' ]
do

if [ $choix = "2" ]
then
read -p 'entrez le nouveau port   ' port
fi

if [ $choix = "3" ]
then
read -p 'entrez la nouvelle IP   ' IP
fi

echo ""
echo "port actuel: $port"
echo "IP actuelle :$IP"
echo ""
echo "si ces valeurs sont correctes,taper 1"
echo "pour modifier le port taper 2"
echo "pour modifier IP taper 3"
echo ""
read -p 'votre choix   ' choix

done


if [ $choix = "1" ]
then
/usr/bin/gst-launch-1.0 -v tcpclientsrc host="$IP" port="$port"  ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false

fi
