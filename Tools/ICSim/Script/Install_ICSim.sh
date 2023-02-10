#!/bin/sh

sudo apt-get install -y gcc make git libsdl2-dev libsdl2-image-dev can-utils  

git clone https://github.com/zombieCraig/ICSim 

cd ICSim

./setup_vcan.sh

make

sudo modprobe vcan 

sudo ip link add dev vcan0 type can

sudo ip link set up vcan0
