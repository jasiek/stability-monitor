# stability-monitor

![hardware](https://raw.githubusercontent.com/jasiek/stability-monitor/master/hardware.jpg)

Arduino Mini Pro-based stability monitor.

Eg. put on your washing machine to figure out when the cycle has finished.

Eats ca. 6 mA of current, and appears to be dominated by a tiny LED onboard the Arduino Mini Pro.

#### DONE
* interrupt-driven operation

#### TODO
* transmit movement readouts using a CC1100-based I2C module
* investigate sleep modes
* investigate voltage regulation
* revisit interrupt-driven operation (do i really need this to operate efficiently?)
* design a PCB
