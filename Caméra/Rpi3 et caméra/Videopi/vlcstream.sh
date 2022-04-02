#!/bin/bash
raspivid -t 300000 -h 240 -w 360 -fps 25 -o -n  | gst-launch -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.103 port= 5000 