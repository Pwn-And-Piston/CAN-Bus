Barbhack 2020 - Car Hacking Workshop
------------------------------------

This software is a fork of "Instrument Cluster Simulator for SocketCAN" aka ICSim
By: OpenGarages <agent.craig@gmail.com>
URI : https://github.com/zombieCraig/ICSim

It was made for the Car Hacking Workshop of the first edition of BARBHACK, the french southernmost hacking event. The purpose of this workshop is to teach some basics on CAN bus hacking (CAN Frame 101, OBDII/UDS/IsoTp protocols)

It extends the basic ICSim with some specific features :
* Some controls are not related to a CAN frame, to simulate sensors/actuators on ECU
* A very basic diag tool is simulated to help understanding UDS protocol
* A few OBD-II and UDS commands are supported
* To learn with fun, a scoring system with few challenges has been implemented

The PDF of the workshop is available here (FR) : https://raw.githubusercontent.com/phil-eqtech/CH-Workshop/master/media/BARBHACK%20-%20Car%20Hacking%20Workshop%20(FR).pdf

![Main UI](https://raw.githubusercontent.com/phil-eqtech/CH-Workshop/master/media/interface.png)
![Controls](https://raw.githubusercontent.com/phil-eqtech/CH-Workshop/master/media/controls.png)

If you are not running on Linux or you don't want to compile the application, a virtual machine (1.1Gb) is available here : https://mega.nz/file/YbRylYBZ#KMW4zd3JmxnkbZCmlqBhkwpty-k6-tacLpci9MnZRms
Login : barbhack - password : 12345678

Challenges
----------
There is 6 challenges to resolve in order to score 100 points :
- Blink the turn signals on the instrument cluster without activating the warning or turn commands (5 pts)
- The speed is limited to 100Km/h. Can you move the needle on the speedometer above this limit ? (10 pts)
- A sensor triggers the lights by night. Can you shut it ? (10 pts)
- Can you get the VIN of the car ? (15 pts)
- The diag tool run a "Routine Control" command. A second command is hidden, can you find it ? (30 pts)
- Can you resolve the "Security Access" challenge of the 0x03 "Diagnostic Session Control" (30 pts)

Compiling
---------
You will need:
* SDL2
* SDL2_Image
* can-utils

You can get can-utils from github or on Ubuntu you may run the follwoing

```
  sudo apt-get install libsdl2-dev libsdl2-image-dev can-utils  
```

Default operation
------------------

You will need a virtual CAN interface to run this software.

You can create it using the "setup_vcan.sh" as sudo
```
  sudo ./setup_vcan.sh
```

or type the following commands :

```
  sudo modprobe can
  sudo modprobe vcan
  sudo ip link add dev vcan0 type vcan
  sudo ip link set up vcan0
```

If you type "ifconfig vcan0" or "ip link" you should see a vcan0 interface.

Once the virtual CAN interface is up and running, type the two following commands in two different terminals :

```
  ./controls vcan0
```
and
```
  ./icsim vcan0
```
Each application can be closed by typing "Ctrl+c" in the terminal window.

Troubleshooting
---------------
* If you have to relaunch the "controls" app, you need to restart "icsim" also, to sync the shared data.
* If the GUI is blinking/glitching, move it away from any open windows and get the focus by clicking in it.
* On the virtual machine, isotprecv and isotpsend commands are not working.

## lib.o not linking
If lib.o doesn't link it's probably because it's the wrong arch for your platform.  To fix this you will
want to compile can-utils and copy the newly compiled lib.o to the icsim directory.  You can get can-utils
from: https://github.com/linux-can/can-utils

## read: Bad address
When running `./icsim vcan0` you end up getting a `read: Bad Address` message,
this is typically a result of needing to recompile with updated SDL libraries.
Make sure you have the recommended latest SDL2 libraries.  Some users have
reported fixing this problem by creating symlinks to the SDL.h files manually
or you could edit the Makefile and change the CFLAGS to point to wherever your
distro installs the SDL.h header, ie: /usr/include/x86_64-linux-gnu/SDL2

There was also a report that on Arch linux needed sdl2_gfx library.
