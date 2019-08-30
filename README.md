# Tennis Ball 

This repository contains C++ and python programs that control a PING ultrasonic distance sensor. I've targetted this for a Raspberry PI 0, an inexpensive but powerful ARM processor.  Often, if people want to avoid hitting something in front of their car in a garage, they will use a tethered tennis ball to bump up against to know that they've pulled in far enough and that they are also a safe distance from the object they want to protect. This little project turns on an LED when a "target" is close enough to the PING sensor.

Parts:
  - Raspberry PI 0 
  - PING 28015
  - 2 1000K ohm resistors
  - 1 LED
  - 1 2N2222 transistor
  - 16 G SD card - can be smaller
  - 110 to USB power suppy and adapter/cables to attach to the PI
  - Zebra Zero Black Ice GPIO case made by C4LABS
  - small prototype circuit board where I mount and interconnect the PING, LED, resisters, and transtor

Assembly - Software:
  1)  Install Stretch Lite from www.raspberrypi.org/downloads/raspbian
      I do headless installs of my PI 0's which, on the publication date
      means that I copy the raspbian image to the SD card plugged into my
      Mac, mount the card and touch the ssh file on the boot partition and
      and create a wpa_supplicant.conf file.
  2)  Boot off the installed image.
  3)  Change the password.
  4)  Change the node name to tennisball.
  5)  sudo apt-get update
  6)  sudo apt-get install python3-pip build-essential cmake git
  7)  sudo pip3 install RPi.GPIO
  8)  git clone https://github.com/mbroihier/tennisball.git
  9)  change to tennisball directory and make the application

```
cd tennisball
make
```

 10)  sudo cp -p tennisball.service /lib/systemd/system/ 
 11)  sudo systemctl enable tennisball


Reboot:
```
sudo shutdown -r now

```
This will start the tennisball application.  When it starts, it will start measuring the distance to the closest object to the PING sensor.  When it detects something at a stable distnace, it will begin lighting an LED indicating that an object is at the "target" distance.  If objects are too far away, the LED will be off.  This will run until the PI is powered off.

There are two versions: a C++ version and a python3 version.  The service runs the C++ version.  They python code is nice for quickly checking out ideas.

[Wiring] (https://github.com/mbroihier/tennisball/tennisball.pdf)