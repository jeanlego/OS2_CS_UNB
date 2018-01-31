#!/bin/bash
mkdir /logs

touch /logs/cloud9.log
touch /logs/tigerVNC.log
touch /logs/noVNC.log

export CC=gcc
export CXX=g++
export DISPLAY=:1

mkdir -p /root/.vnc
echo letmein | vncpasswd -f >> /root/.vnc/passwd
chmod 600 /root/.vnc/passwd

node /cloud9/server.js --listen 0.0.0.0 --port 8080 -w /workspace --collab -a root:letmein > /logs/cloud9.log &

websockify -D --web=/usr/share/novnc/ 6901 localhost:5901 >> /logs/noVNC.log &
vncserver -kill :1 || rm -rfv /tmp/.X*-lock /tmp/.X11-unix
vncserver :1 -depth 24 -geometry 1280x1024 >> /logs/tigerVNC.log &

xset -dpms &
xset s noblank &
xset s off &
/usr/bin/startxfce4 --replace
