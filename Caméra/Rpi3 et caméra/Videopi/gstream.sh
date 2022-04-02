#!/bin/bash
raspivid -t 300000 -h 240 -w 360 -fps 25 -o -n  | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264 