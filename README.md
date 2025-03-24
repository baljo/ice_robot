# Ice Curing Rover

## The Problem

Too many persons get injured (or worse) due to icy conditions, in the US alone there are [tens of thousands of injuries](https://www.bls.gov/opub/ted/2016/42480-work-injuries-involved-ice-sleet-or-snow-in-2014.htm) involving ice, sleet, or snow. Typically the city or state is responsible for ensuring public roads are safe to use, but the responsibility moves to you at the start of your own driveway, and you thus need to ensure it is safe. For this you can of course salt or sand your icy driveway manually, similarly as you can mow your lawn manually, vacuum your home manually, doing the dishes by hand, etc. Curing your driveway from ice might for some be fun and for others tedious, if tedious then look no further but instead continue reading for a solution!

![](/images/Icy_driveway.jpg)
--*Finland, March 2025*--


## The Solution

Meet Ice Beat-Le, a symphatethic looking rover which is beating ice by spreading salt, sand, or any other material on your driveway or lawn. The rover drives with all six wheels, and it navigates with its IR-eyes locking in on an IR-Beacon. 

As MCU for the rover I chose the Particle Photon 2 as it's so versatile and powerful, but still simple to work with. For the IR beacon I used a Propeller Activity Board from Parallax as it has an D/A chip built in. If I'd had another Photon 2 available, I'd probably redesigned the beacon circuit and program to use PWM (Pulse Width Modulation) instead of the D/A chip.

![](/images/IMG-20250321-WA0001_compr.jpg)



### How does it work?

While the rover might look complex, with several electronic components, motors, servo and many mechanical parts, it is actually in the end not complicated at all, perhaps even minimalistic. The reason for this is that there are no unnecessary bells and whistles, the rover is doing what it was planned to do, nothing more, nothing less. The journey while designing and building the end product was though far from simple, with many obstacles on the road.

#### Navigation

Navigation is performed with the help of an IR beacon transmitting a 38 kHz IR-signal of varying intensity. This IR-signal is using the Sony protocol, same as many TV remote controls are using. Unfortunately the IR-receivers - "eyes" - on the rover can only detect if there is - or is not - a modulated IR light present, they can't measure the intensity of the light. This means it isn't possible to navigate only using a constant IR-signal as there's no easy way for the rover to know if it's heading to the left or to the right of the signal beam.  

![](/images/Ice%20Beat-Le-1.png)
--*Rover on a driveway*--

  

To solve this problem, I found a [solution](https://learn.parallax.com/courses/ir-beacon-seeking-activitybot-with-blocklyprop/) involving both transmitting the IR-signal itself as well as changing the intensity of it. By varying the strength of the beacon transmitter and let the rover measure how many times the beacon is seen over a period while its light is varying, the rover is able to navigate.

To generate both the modulated (pulsed) IR signal AND the varying brightness, two different signals are fed into the LED at the same time. The positive leg of the LED connects to an I/O pin, where the code is sending it pulses by turning it on and off 38000 times per second. The negative leg of the LED connects to a D/A-pin, which is changing its voltage output from 0V (full brightness for the LED) to 1.6V (nearly off). Why is 0V bright and 1.6V dim? Because this is where the negative leg is connected. The result is that the “Sawtooth”-like output of D/A is subtracted from the 3.3V pulses from the I/O pin:


![](/images/Ice%20Beat-Le-2.png)

As mentioned, the rover measures for each IR receiver how many time it has seen the IR-light from the beacon, and if one has seen it less times than the other, the rover turns the opposite direction. E.g., as the driveway sketch further above illustrates, the left eye is slightly farther away from the beacon than the right eye, and thus collects less light, leading to that it needs to turn to the right. As the IR-signals might be slightly erratic due to external interference, a separate moving average window is used for each receiver.


![](/images/Ice%20Beat-Le-3.png)

![](/images/IR_beacon_case.png)

#### Sanding/Salting 

In the name of simplicity, and instead of inventing a salting mechanism needing a strong motor, I decided to use a force available to all of us for free → gravity. Sand, salt, or any other suitable material is poured into the container which has a hole in the bottom. To avoid that the sand falls away directly, a servo-controlled lid is opened or closed within the main control program. The sand falls down on the spreader which spreads it over approximately a half meter in width while the rover is moving forward. If large particles are used, there's a risk of them getting stuck at the bottom. However, as the rover doesn't have differential steering, it twitches a bit when alternating between driving, turning, and coasting, and I found out that this leads to that potential clogs are easily dissolved.


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
- separate power supply recommended for the motors, max 7.5 VDC
  - remember to tie GND together if using separate power supplies
- large breadboard
- 2 servo headers to connect to the HB-25 motor controllers
- 10kΩ resistors, 2 pcs
- assorted wires


**IR Beacon**
- [Propeller Activity Board](https://www.parallax.com/product/propeller-activity-board-wx/), or any other microcontroller board with an D/A-chip. It is also possible to use PWM (Pulse Width Modulation) to replicate change in intensity.
- [TSAL6200 IR LED](https://www.partco.fi/en/leds/special-purpose-leds/17474-tsal6200.html), TSAL6100 even better as it has narrower angle
- [IRLZ44N Mosfet](https://www.partco.fi/en/electronic-components/actives/bjt-and-fet/switching-fets-n-channel/5057-irlz44n.html)
- 2.2Ω Resistor
- 10kΩ Resistor
- 5V power for the microcontroller, e.g. a powerbank
- assorted wires

Do note that the program is responsible for turning the IR LED on and off, if it's on all the time, it will burn the LED or the resistor. The Mosfet is needed to push out far more current than is possible right out of a microcontroller pin.


## Assembly Instructions

- Assemble the rover itself according to instructions, my rover came preassembled as it was a used one.

- 3D-print the different components, most of them do not need supports if they are rotated properly. For most parts I used an infill of 10-20 %. 
- Feel free to redesign the components according to your platform and needs. Remember to add tolerances when designing pieces that need to fit together!

### Wiring and Schematics

#### Rover Electronics

- Photon 2 connections

| Photon 2 | To                 |   
|--------- |--------------------|
| D15      | Left motor signal  |  
| D1       | Right motor signal |  
| D2       | Left IR signal     |  
| D10      | Right IR signal    |   
| D16      | Lid servo signal   |  
| VUSB     | Lid servo power    | 

- Fritzing diagram 

![](/images/ice_robot_bb.png)

- Close-up photo, do note that the lid controlling servo is left out of the photo for clarity, it is connected to VUSB and D16.

![](/images/Rover%20electronics.png)






#### IR Beacon

- Fritzing diagram. In my setup I'm using the Propeller Activity Board as it has a D/A-chip.

![](/images/IR_beacon_bb.jpg)

- Close-up photo. To narrow the infrared beam a bit, the IR LED is equipped with a plastic straw enclosed in black electronic tape.

![](/images/Beacon%20electronics.png)

## Software Description

Below a description of the main functions.

### IR Beacon

The beacon program is very simple, it pulses out IR light with a frequency of 38 kHz, in this case using pin 11 on the Propeller MCU. Every pulse is 3 milliseconds in length. The intensity is changed between nearly off until fully lit (330 = 3.3V).

```
// ------ Libraries and Definitions ------
#include "simpletools.h"
#include "abvolts.h"

// ------ Global Variables and Objects ------
int dimmer;

// ------ Main Program ------
int main() {
  da_init(26, 27);
  while(1) {
    for (dimmer = 0; dimmer < 160; dimmer += (10)) {
      da_out(0, (dimmer* 256 / 330));
      freqout(11, 3, 38000);
    }
  }
}

```

### Photon 2


- The function `IR_read` reads the IR sensors 96 times per loop and accumulates the pulses read per sensor. The higher the number is, the closer or more aligned the sensor is to the beacon.

```
// Read the IR sensors
// leftOut and rightOut are passed by reference
// They will be updated with the number of times the IR sensor was triggered
// The higher the number, the closer the sensor is to an object

void IR_read(int &leftOut, int &rightOut) {
  int leftIR = 0;
  int rightIR = 0;
  
  // Sample 96 times (16 * 6)
  for(int n = 0; n < 96; n++) {
    leftIR  += !digitalRead(LEFT_IR_PIN);                   // Read the digital value of the IR sensor
    rightIR += !digitalRead(RIGHT_IR_PIN);                  // Read the digital value of the IR sensor
    delay(1);                                               // 1 ms between samples
  }
  
  // Assign results to the provided references
  leftOut  = leftIR;
  rightOut = rightIR;
}
```

- The function `drive` drives the rover by the speed received for each side. The motors are off when the value is 90, and as they need to rotate in opposite directions the given speed is added to 90 for the left motors, and subtracted from 90 for the right motors. 

```
void drive(int left_speed, int right_speed, int time) {
  left.write(90 + left_speed);    // Move left motor forward
  right.write(90 - right_speed);  // Move right motor forward
  delay(time);                    // Delay selected time
}
```

- The main loop consists of reading the IR sensors, calculate a rolling average for each of them, and based on possible difference between the readings turn right or left. As the rover's minimum speed is too fast indoors, it is alternating between driving forware, coasting, or turning.

```
  IR_read(leftIR, rightIR);                                 // Read the IR sensors  
----------------------
  int left_avg = calculateRollingAverage(leftIR, 0);        // Rolling average for Sensor 1
  int right_avg = calculateRollingAverage(rightIR, 1);      // Rolling average for Sensor 2
  int diff = coeff * (left_avg - right_avg);                // Calculate the difference between the IR sensors
----------------------
  int left_avg = calculateRollingAverage(leftIR, 0);        // Rolling average for Sensor 1
  int right_avg = calculateRollingAverage(rightIR, 1);      // Rolling average for Sensor 2
  int diff = coeff * (left_avg - right_avg);                // Calculate the difference between the IR sensors
----------------------
// If either leftIR or rightIR is very low, don't move
  while (left_avg <= 2 || right_avg <= 2 && left_avg != right_avg) {
    Log.info("Seeking IR signal");

    IR_read(leftIR, rightIR);
    left_avg = calculateRollingAverage(leftIR, 0);          // Rolling average for Sensor 1
    right_avg = calculateRollingAverage(rightIR, 1);        // Rolling average for Sensor 2
    diff = coeff * (left_avg - right_avg);                  // Calculate the difference between the IR sensors

    Log.info("   Left avg: %2d,   Right avg: %2d,   Diff: %3d", left_avg, right_avg, left_avg - right_avg);
  }

  if (diff == 0) {                                          // If the difference is 0, drive straight
    drive(leftSpeed, rightSpeed, drive_time);
  } else if (diff > 0) {                                    // If the difference is positive, turn right
    drive(leftSpeed, rightSpeed + diff, drive_time);
  } else {                                                  // If the difference is negative, turn left
    drive(leftSpeed - diff, rightSpeed, drive_time);
  }

  drive(leftSpeed / 3, rightSpeed / 3, drive_time);         // Drive forward at a slower speed, otherwise the robot will drive too fast
```

## Final Demonstration

A YouTube video is found [here](https://youtu.be/t5bE52Lg39M).


![](/images ****************/Ice_Beat_Le.gif)



## Conclusion & Next Steps


### Issues overcome

As mentioned earlier, while the end result is minimalistic, the road leading to the goal was not simple or straight, and it indeed had a few bumps and cul-de-sacs. Finding a solution to the challenge of outdoors navigation took some time as I needed to purchase different IR-LEDs and receivers and test them in practice. Learning Fusion 360 from scratch, designing, and 3D-printing the 12 different components also took a significant amount of time, many iterations and roughly 1 kg of filament out of which 80 % was scrapped. 

### Future enhancements

This first version of the rover is not completely autonomous, and for safety reasons it moves only while the IR-beacon is transmitting. Right now it only moves forward (and of course turns left or right), but it would be quite easy to have it travel forward for a given time, and reverse the same amount of time. Also quite easy to solve would be to have it cover a wider area, either by larger spreader mechanism or by programming it to travel in different "lanes". To make the rover fully autonomous, it would need bumper switches and/or distance measuring sensors to avoid obstacles, and some type of odometer. A differential GPS with centimeter precision would solve many problems, but would also come with a hefty price tag. 

For a permanent installation, the breadboard obviously would be replaced with a custom PCB or at least using a protoboard with soldered wires. In addition, the two separate batteries could be replaced with a large capacity battery. 

### Final Conclusions

This Ice Curing Rover, affectionately called Ice Beat-Le, is a six-wheeled autonomous vehicle that spreads sand or salt on icy driveways, helping reduce winter slip hazards. At the heart of the rover is the Particle Photon 2, whose powerful yet beginner-friendly platform made it easy to prototype, program, and control the robot’s complex behaviors with minimal overhead. Navigating via a modulated IR-beacon and using a gravity-fed dispenser controlled by a servo, the rover is a compact and cost-effective solution built with 3D-printed parts and off-the-shelf components. The Photon 2’s combination of real-time control, built-in connectivity, and versatility made it the ideal choice for this project - especially for balancing simplicity with performance. With a few additions, such as obstacle sensors or lane logic, the rover could easily evolve into a fully autonomous ice-clearing assistant.

While the road was bumpy to a degree, it led eventually to the goal, and is now ice free! 



