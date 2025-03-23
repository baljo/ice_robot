# Ice Curing Rover

## The Problem

Too many persons get injured (or worse) due to icy conditions, in the US alone there are [tens of thousands of injuries](https://www.bls.gov/opub/ted/2016/42480-work-injuries-involved-ice-sleet-or-snow-in-2014.htm) involving ice, sleet, or snow. Typically the city or state is responsible for ensuring public roads are safe to use, but the responsibility moves to you at the start of your own driveway, and you thus need to ensure it is safe. For this You can of course salt or sand your icy driveway manually, similarly as you can mow your lawn manually, vacuum your home manually, doing the dishes by hand, etc. Curing your driveway from ice might for some be fun and for others tedious, if tedious then look no further but instead continue reading!

![](/images/Icy_driveway.jpg)

--*Finland, March 2025*--


## The Solution

Meat Ice Beat-Le, a symphatethic looking rover which is beating ice by spreading salt, sand, or any other material on your driveway or lawn. The rover drives with all six wheels, and it navigates with its IR-eyes locking in on an IR-Beacon. 

![](/images/IMG-20250321-WA0001_compr.jpg)



### How does it work?

While the rover might look complex, with several electronic components, motors, servo and many mechanical parts, it is actually in the end not complicated at all, perhaps even minimalistic. The reason for this is that there are no unnecessary bells and whistles, the rover is doing what it was planned to do, nothing more, nothing less. The journey while designing and building the end product was though far from simple, with many obstacles on the road.

#### Navigation

Navigation is performed with the help of an IR beacon transmitting a 38 kHz IR-signal of varying intensity. This IR-signal is using the Sony protocol, same as many TV remote controls are using. Unfortunately the IR-receivers - "eyes" - on the rover can only detect if there is - or is not - a modulated IR light present, they can't measure the intensity of the light. This means it isn't possible to navigate only using a constant IR-signal as there's no easy way for the rover to know if it's heading to the left or to the right of the signal beam.  

![](/images/Ice%20Beat-Le-01.png)

--*Rover on a driveway*--

  

To solve this problem, I found a [solution](https://learn.parallax.com/courses/ir-beacon-seeking-activitybot-with-blocklyprop/) involving both transmitting the IR-signal itself as well as changing the intensity of it. By varying the strength of the beacon transmitter and let the rover measure how many times the beacon is seen over a period while its light is varying, the rover is able to navigate.

To generate both the modulated (pulsed) IR signal AND the varying brightness, two different signals are fed into the LED at the same time. The positive leg of the LED connects to an I/O pin, where the code is sending it pulses by turning it on and off 38000 times per second. The negative leg of the LED connects to a D/A-pin, which is changing its voltage output from 0V (full brightness for the LED) to 1.6V (nearly off). Why is 0V bright and 1.6V dim? Because this is where the negative leg is connected. The result is that the “Sawtooth”-like output of D/A is subtracted from the 3.3V pulses from the I/O pin:


![](/images/Ice%20Beat-Le-2.png)

As mentioned, the rover measures for each IR receiver how many time it has seen the IR-light from the beacon, and if one has seen it less times than the other, the rover turns the opposite direction. E.g., as the driveway sketch further above illustrates, the left eye is slightly farther away from the beacon than the right eye, and thus collects less light, leading to that it needs to turn to the right. As the IR-signals might be slightly erratic due to external interference, a separate moving average window is used for each receiver.


![](/images/Ice%20Beat-Le-3.png)

#### Sanding/Salting

In the name of simplicity, and instead of inventing a salting mechanism needing a strong motor, I decided to use a force available to all of us for free → gravity. Sand, salt, or any other suitable material is poured into the container which has a hole in the bottom. To avoid that the sand falls away directly, a servo-controlled lid is opened or closed within the main control program. The sand falls down on the spreader which spreads it over approximately a half meter in width while the rover is moving forward. If large particles are used, there's a risk of them getting stuck at the bottom. The rover is however twitching a bit by alternating between driving, turning, and coasting, and I found out that this leads to that potential clogs are dissolved.


![](/images/Sanding%20mechanism.png)

![](/images/Dome%20top%20and%20bottom.png)

![](/images/Container%20and%20servo.png)


## Materials Required

**Rover**
- [Particle Photon 2](https://store.particle.io/products/photon-2?srsltid=AfmBOopQNaLMpaWTah99rLqtw8SrwQJ14wpPAscubC-w2jhXiWBzn8VZ)
- Dagu Wild Thumper 6WD (my rover included 6 motors and 2 HB-25 motor controllers)
- 38 kHz Infrared IR Receiver, e.g. [this](https://www.jameco.com/z/TSOP34138-Vishay-Infrared-IR-Receiver-38Khz-Carrier-Frequency-Use-with-Parallax-350-00017-Boe-Sumo_2109691.html?srsltid=AfmBOopVCaDbtUFmN_kAVQGARot6k9othqFZ7YneEFLCrC20eo9ZwV60) 
- [Kitronik Linear Actuator Kit](https://kitronik.co.uk/products/2595-linear-actuator?_pos=3&_sid=e648f346a&_ss=r)
- 5V power for the Photon 2, e.g. a small powerbank
- separate power source recommended for the motors, max 7.5 VDC
- large breadboard
- 2 servo headers to connect to the HB-25 motor controllers
- 10kΩ resistors, 2 pcs
- assorted wires


**IR Beacon**
- [Propeller Activity Board](https://www.parallax.com/product/propeller-activity-board-wx/), or any other microcontroller board with an D/A-chip
- [TSAL6200 IR LED](https://www.partco.fi/en/leds/special-purpose-leds/17474-tsal6200.html), TSAL6100 even better as it has narrower angle
- [IRLZ44N Mosfet](https://www.partco.fi/en/electronic-components/actives/bjt-and-fet/switching-fets-n-channel/5057-irlz44n.html)
- 2.2Ω Resistor
- 10kΩ Resistor
- 5V power for the microcontroller, e.g. a powerbank
- assorted wires

Do note that the program is responsible for turning the IR LED on and off, if it's on all the time, it will burn the LED or the resistor. The Mosfet is needed to push out far more current than is possible right out of a microcontroller pin.




## Assembly Instructions
Detailed step-by-step build process.

## Wiring and Schematics
Clear visuals and schematics.

## Software Setup
Code installation and usage instructions.

## Final Demonstration

![](/images/Ice_Beat_Le.gif)

Videos, GIFs, or images demonstrating the final result.

## Troubleshooting & FAQ
Common issues and solutions.

## Conclusion & Next Steps

This minimalistic first version of the rover is not completely autonomous, and it moves only while the IR-beacon is transmitting. Right now it only moves forward (and of course turns left or right), but it would be quite easy to have it travel forward for a given time, and reverse the same amount of time. To make the rover autonomous, it would need bumper switches and/or distance measuring sensors to avoid obstacles. A differential GPS with centimeter precision would solve many problems, but comes with a hefty price tag.  


Learning Fusion 360 from scratch, designing, and 3D-printing the 12 different components took a lot of time, many iterations and roughly 1 kg of filament out of which 80 % was scrapped.
Possible future enhancements.
