# E46Track - Raspberry Deployment

## Why is there a Raspberry edition?

Unfortunately my phone (Nokia 7 plus) adds some delay (quite much, even 1+ second) periodically on the socket which makes the playback rather unpleasant.
During my tests java.socket, java.nio, and even my Qt app produced the issue, I tried with several thread priority, etc, but I couldn't fix the issue.

Raspberry deployment would work with iPhone too.

## How it is working?

1. Create a WiFi HotSpot with your phone.
2. Connect the Raspberry to this network.
3. Open <raspberry-ip>:5000 from your Phone, e.g: with Google Chrome.
4. Use the Add to home screen functionality, and you will have an App.

From this E46Launcher app you can start the E46Track app, download the recordings and power off the pi.

## Known issues

1. E46Track/WiFi: can't type ip address from phone. (Workaround use a computer for this or edit the config manually ~/.config/E46Track/App.conf)
2. E46Track/Replay view: the filedialog is not properly streamed with qt5-webglplugin. (Workaround: use a computer or android phone for replaying.)

## Deployment

Install Archlinux ARM on your Pi. (For Pi3 follow the instructions here: https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-3)

SSH with the alarm user:
```
$ ssh alarm@<raspberry-pi-ip-address>
```

Install dependencies:
```
$ su root
$ pacman -S base-devel git cmake go qt5-base qt5-declarative qt5-quickcontrols qt5-quickcontrols2 qt5-serialport qt5-svg qt5-tools qt5-webglplugin qt5-websockets ttf-dejavu
```

Clone E46Track:
```
# alarm user

$ cd ~
$ git clone https://github.com/tomicooler/E46Track.git
```

Build E46Track:
```
# alarm user

cd E46Track
mkdir build-App
cmake ../App
make
```

Copy & enable systemd services:
```
$ su root
$ cp /home/alarm/E46Track/raspberry/*.{socket,service} /etc/systemd/system/.
$ systemctl enable e46track.service
$ systemctl enable bbpi.socket
$ systemctl enable e46launcher.service
```

Install udev rule (If you will be using the Serial Interface, K+DCAN Cable):
```
$ su root
$ cp /home/alarm/E46Track/raspberry/42-latency-ttyUSB0.rules /etc/udev/rules.d/.
$ gpasswd -a alarm uucp
```

Networking:

/etc/netctl/wlan0-YourPhoneHotSpot (first use IP=dhcp then fill the static ip)
```
Description='Create a HotSpot with your phone'
Interface=wlan0
Connection=wireless
Security=wpa
ESSID=<YourHotSpotSSID>
IP=static
Address="192.168.43.227/24"
Gateway="192.168.43.50"
Key=YourHotSpotSecretKey
```

```
systemctl enable netctl-auto@wlan0.interface
```

/etc/netctl/wlan0-WiFi_OBDII (If you will be using WIFI_OBDII interface)
```
Description="WiFi_OBDII"
Interface=wlan1
Connection=wireless
Security=none
ESSID=WiFi_OBDII
IP=dhcp
```

```
systemctl enable netctl-auto@wlan1.interface
```