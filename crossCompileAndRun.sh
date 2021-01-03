#!/bin/bash

# Before running create ssh key with:
# ssh-keygen -t rsa -b 2048
# ssh-copy-id id@server
# scp C:/Users/lukas/.ssh/id_rsa.pub pi@192.168.0.60:/home/pi/.ssh/
# ON RASPBERRY
# echo `cat id_rsa.pub` >> ~/.ssh/authorized_keys

IP="192.168.0.60"
USERNAME="pi"
OUTPUT="a.out"
BINDIR="/home/pi/spider"

set -e

#../../tools/SysGCC/bin/arm-linux-gnueabihf-g++.exe *.cpp -o $OUTPUT

 ../../tools/SysGCC/bin/arm-linux-gnueabihf-g++.exe -std=c++17  -pthread *.cpp Robot.hpp CallBackTimer.hpp -o $OUTPUT

scp $OUTPUT $USERNAME@$IP:$BINDIR

ssh -t $USERNAME@$IP "cd $BINDIR && ./$OUTPUT"