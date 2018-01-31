#!/bin/bash
rm -Rf /workspace/logs
mkdir -p /workspace/logs

touch /workspace/logs/cloud9.log
touch /workspace/logs/tigerVNC.log
touch /workspace/logs/noVNC.log

export CC=gcc
export CXX=g++
export DISPLAY=:1

mkdir -p /root/.vnc
echo letmein | vncpasswd -f >> /root/.vnc/passwd
chmod 600 /root/.vnc/passwd " >> /startup.sh

node /cloud9/server.js --listen 0.0.0.0 --port 8080 -w /workspace --collab -a root:letmein > /workspace/logs/cloud9.log &" >> startup.sh

websockify -D --web=/usr/share/novnc/ 6901 localhost:5901 >> /workspace/logs/noVNC.log &
vncserver -kill :1 || rm -rfv /tmp/.X*-lock /tmp/.X11-unix
vncserver :1 -depth 24 -geometry 1280x1024 >> /workspace/logs/tigerVNC.log &" >> startup.sh

xset -dpms &
xset s noblank &
xset s off &
/usr/bin/startxfce4 --replace
