# This Dockerfile is used to build an headles vnc image based on Fedora with cloud9
FROM fedora:latest

# ------------------------------------------------------------------------------
# Install base
RUN dnf -y update '*' --refresh && \
dnf install -y glibc-static tmux python libevent-devel ncurses-devel clang perl-List-MoreUtils \
time @development-tools @virtualization zip clang curl git libxml++-devel libX11-devel libXft-devel fontconfig \
cairo-devel automake cmake flex bison ctags gdb perl valgrind vim sudo wget which net-tools bzip2 \
gdb-gdbserver numpy tigervnc-server python novnc python3-websockify python3-numpy gettext nasm \
qemu firefox

ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8'

RUN dnf -y -x gnome-keyring --skip-broken groups install "Xfce" && dnf -y groups install "Fonts"
RUN dnf erase -y *power* *screensaver* && rm /etc/xdg/autostart/xfce-polkit* && /bin/dbus-uuidgen > /etc/machine-id

RUN ln -s /lib64 /usr/lib/x86_64-linux-gnu

# Install Node.js
RUN curl -L https://raw.githubusercontent.com/c9/install/master/install.sh | bash
RUN dnf install -y nodejs

# Install Cloud9
RUN git clone https://github.com/c9/core.git /cloud9
RUN /cloud9/scripts/install-sdk.sh
RUN sed -i -e 's_127.0.0.1_0.0.0.0_g' /cloud9/configs/standalone.js

#cleanup
RUN mkdir -p /workspace

RUN echo "#!/bin/bash " > startup.sh

RUN echo "USER_NAME=$1 " >> startup.sh
RUN echo "PASSWD=$2 " >> startup.sh

RUN echo "rm -Rf /workspace/logs " >> startup.sh
RUN echo "mkdir -p /workspace/logs " >> startup.sh

RUN echo "touch /workspace/logs/cloud9.log " >> startup.sh
RUN echo "touch /workspace/logs/tigerVNC.log " >> startup.sh
RUN echo "touch /workspace/logs/noVNC.log " >> startup.sh

RUN echo "export CC=gcc " >> startup.sh
RUN echo "export CXX=g++ " >> startup.sh
RUN echo "export DISPLAY=:1 " >> startup.sh

RUN echo "mkdir -p /root/.vnc " >> startup.sh
RUN echo "echo PASSWD | vncpasswd -f >> /root/.vnc/passwd " >> startup.sh
RUN echo "chmod 600 /root/.vnc/passwd " >> /startup.sh

RUN echo "node /cloud9/server.js --listen 0.0.0.0 --port 8080 -w /workspace --collab -a $USER_NAME:$PASSWD > /workspace/logs/cloud9.log &" >> startup.sh

RUN echo "websockify -D --web=/usr/share/novnc/ 6901 localhost:5901 >> /workspace/logs/noVNC.log & " >> startup.sh
RUN echo "vncserver -kill :1 || rm -rfv /tmp/.X*-lock /tmp/.X11-unix " >> startup.sh
RUN echo "vncserver :1 -depth 24 -geometry 1280x1024 >> /workspace/logs/tigerVNC.log &" >> startup.sh

RUN echo "xset -dpms & " >> startup.sh
RUN echo "xset s noblank & " >> startup.sh
RUN echo "xset s off & " >> startup.sh
RUN echo "/usr/bin/startxfce4 --replace " >> startup.sh

RUN chmod +x startup.sh

# expose port and directory
VOLUME /workspace
EXPOSE 8080 5901 6901

ENTRYPOINT ["/bin/sh", "./startup.sh"]
CMD ["root", "letmein"]
