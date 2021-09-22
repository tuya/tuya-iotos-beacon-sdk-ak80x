[TOC]

</br>

### 1. Introduction

Bluez is the default Bluetooth protocol stack on Linux. It should be present and installed on your Linux distribution. If not, building and installing from source is not too difficult:

- Download the latest stable source release of Bluez from here. Unzip the compressed file you downloaded.
- Install the headers and libraries required for Bluez compilation:

</br>

### 2. Download And Install

I follow the blog ([Installing Bluez 5.44 onto Raspbian?][#1]) to install bluez-5.50.

Download the most recent version from the official page:[http://www.bluez.org/download/][#2].

For example, at the time of writing it was 5.50, so I used(on my pi):
```
wget http://www.kernel.org/pub/linux/bluetooth/bluez-5.50.tar.xz
```

Then I extracted it and built it:
```
tar -xf bluez-5.50.tar.xz
cd bluez-5.50
```

**Read the README! **It lists the dependencies and the configure switches:

Install the dependencies first: (glib, dbus, libdbus, udev, etc.)
```
sudo apt install libdbus-1-dev libudev-dev libical-dev libreadline-dev
```
**note:** If you do not install the libdbus-1-dev, you will later get this strange error:
> configure: error: D-Bus >= 1.6 is required

once you've installed dependencies, you can configure switches:
```
./configure --prefix=/usr --mandir=/usr/share/man --sysconfdir=/etc --localstatedir=/var  --enable-experimental
```

then do:
```
make
sudo make install
```

It takes maybe 10 minutes to compile. After installing, you should find  bluetoothd in `/usr/libexec/bluetooth`. You should also see bluetoothd in `/usr/lib/bluetooth`.

Go to each of these directories and type
```
./bluetoothd --version
```

You'll note that the one in libexec is new and the one in lib is old.

In order to make sure that d-bus is talking to you **new** BlueZ 5.50 and not your **old** BlueZ 5.43, you need to tell systemd to use the new bluetooth daemon:
```
sudo vim /lib/systemd/system/bluetooth.service
```

Make sure the `exec.start` line points to your new daemon in `/usr/libexec/bluetooth`.

For me, that wasn't enough. No matter what, upon restart I always got bluetoothd 5.43... So I just created a symlink from the old one to the new.

First rename the old file:
```
sudo mv /usr/lib/bluetooth/bluetoothd /usr/lib/bluetooth/bluetoothd-543.orig
```

Create the symlink:
```
sudo ln -s /usr/libexec/bluetooth/bluetoothd /usr/lib/bluetooth/bluetoothd
sudo systemctl daemon-reload
```

That should do it.

</br>

### 3. How to use the bluez and hcitool

#### 3.1 setup bluetooth service
Start the bluetooth service and enable automatic startup, assuming you're using systemd as the init daemon:
```
sudo systemctl start bluetooth.service
sudo systemctl enable bluetooth.service
```

#### 3.2 hcitool scan for bluetooth devices
Before start scanning make sure that your bluetooth device is turned on and not blocked, you can check that with the rfkill command:
```
sudo rfkill list
```

If the bluetooth device is blocked (soft or hard blocked), unblock it with the rfkill command again:
```
sudo rfkill unblock bluetooth
```

Bring up the bluetooth device with hciconfig command and start scanning, make sure the target device's bluetooth is on and It's discoverable:
```
sudo hciconfig hci0 up
hcitool scan
```

Wait few moment to complete the hcitool scan or hcitool lescan, the results will be something like bellow:

![][#6]

![][#8]

Here 00:1A:7D:DA:71:0A is the bluetooth MAC address and SHEN-PC is the name of the bluetooth device, i.e. an PC.

**note:** use `hcitool lescan` will forever scan ble devices, if use ctrl+c stop it, it will show error(ref to [LINKS 4][#7] to solve):
> hcitool lescan
> Set scan parameters failed: Input/output error

#### 3.3 bluetooth service discovery
Now we have the bluetooth MAC address of the target device, use the `sdptool` command to know which services (like DUN, Handsfree audio) are available on that target device.
```
sdptool browse 28:ED:6A:A0:26:B7
```

You can also use the interactive `bluetoothctl` tool for this purpose.

If the target device is present, you can ping it with l2ping command, requires root privilege:
```
➜  bluez-5.50  sudo l2ping 94:87:E0:B3:AC:6F
Ping: 94:87:E0:B3:AC:6F from B8:27:EB:8E:CC:51 (data size 44) ...
44 bytes from 94:87:E0:B3:AC:6F id 0 time 53.94ms
44 bytes from 94:87:E0:B3:AC:6F id 1 time 77.12ms
44 bytes from 94:87:E0:B3:AC:6F id 2 time 38.63ms
44 bytes from 94:87:E0:B3:AC:6F id 3 time 46.13ms
44 bytes from 94:87:E0:B3:AC:6F id 4 time 59.96ms
5 sent, 5 received, 0% loss
```

So, bluetooth service discovery is useful to determine the type of the device, like if it's a bluetooth mp3 player or it's a keyboard.




</br>

### LINKS

- [1.Bluetooth on Modern Linux by Szymon Janc][#3]
- [2.dbus-python tutorial][#4]
- [3.Linux bluetooth setup with bluez and hcitool][#5]
- [4.hcitool lescan shows I/O error][#7]


[#1]:https://raspberrypi.stackexchange.com/questions/66540/installing-bluez-5-44-onto-raspbian/74712
[#2]:http://www.bluez.org/download/
[#3]:https://www.youtube.com/watch?v=tclS9arLFzk
[#4]:https://dbus.freedesktop.org/doc/dbus-python/tutorial.html
[#5]:https://www.pcsuggest.com/linux-bluetooth-setup-hcitool-bluez/
[#6]:./rfkill.png
[#7]:https://stackoverflow.com/questions/22062037/hcitool-lescan-shows-i-o-error
[#8]:./hciscan.png
