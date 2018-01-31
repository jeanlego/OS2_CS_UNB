#!/bin/bash
mkdir -p /logs

touch /logs/cloud9.log
touch /logs/tigerVNC.log
touch /logs/noVNC.log
touch /logs/credentials.log

STARTUP_DEAFULT_USERNAME="root"
STARTUP_DEFAULT_PASSWORD="letmein"
STARTUP_DEFAULT_CLOUD9_ROOT_FOLDER="/workspace"

STARTUP_USERNAME=$USERNAME
STARTUP_PASSWORD=$PASSWORD
STARTUP_CLOUD9_ROOT_FOLDER=$CLOUD9_ROOT_FOLDER

if [[ -z "$STARTUP_USERNAME" ]]; then
	STARTUP_USERNAME=$STARTUP_DEAFULT_USERNAME
fi

if [[ -z "$PASSWORD" ]]; then
	STARTUP_PASSWORD=$STARTUP_DEFAULT_PASSWORD
fi

if [[ -z "$CLOUD9_ROOT_FOLDER" ]]; then
	STARTUP_CLOUD9_ROOT_FOLDER=$STARTUP_DEFAULT_CLOUD9_ROOT_FOLDER
fi

echo "${0##*/}: STARTUP_USERNAME: $STARTUP_USERNAME" |& tee -a /logs/credentials.log
echo "${0##*/}: STARTUP_PASSWORD: $STARTUP_PASSWORD" |& tee -a /logs/credentials.log
echo "${0##*/}: STARTUP_CLOUD9_ROOT_FOLDER: $STARTUP_CLOUD9_ROOT_FOLDER" |& tee -a /logs/cloud9.log

export CC=gcc
export CXX=g++
export DISPLAY=:1

mkdir -p /root/.vnc
echo "$STARTUP_PASSWORD" | vncpasswd -f >> /root/.vnc/passwd
chmod 600 /root/.vnc/passwd

node /cloud9/server.js --listen 0.0.0.0 --port 8080 -w "$STARTUP_CLOUD9_ROOT_FOLDER" --collab -a "$STARTUP_USERNAME":"$STARTUP_PASSWORD" > /logs/cloud9.log &

websockify -D --web=/usr/share/novnc/ 6901 localhost:5901 >> /logs/noVNC.log &
vncserver -kill :1 || rm -rfv /tmp/.X*-lock /tmp/.X11-unix
vncserver :1 -depth 24 -geometry 1280x1024 >> /logs/tigerVNC.log &

xset -dpms &
xset s noblank &
xset s off &
/usr/bin/startxfce4 --replace
