# Ice Curing Rover

## The Problem

Too many persons get injured (or worse) due to icy conditions, in the US alone there are [tens of thousands of injuries](https://www.bls.gov/opub/ted/2016/42480-work-injuries-involved-ice-sleet-or-snow-in-2014.htm) involving ice, sleet, or snow. TTypically, the city or state is responsible for ensuring public roads are safe, but responsibility shifts to you at the start of your own driveway. Thus, you need to ensure it is safe yourself. You can, of course, salt or sand your icy driveway manually, just as you can mow your lawn, vacuum your home, or wash dishes by hand. While curing your driveway from ice might be enjoyable for some, it’s tedious for others. If you find yourself in the latter group, read on for a solution!

![](/images/Icy_driveway.jpg)
--*Finland, March 2025*--


## The Solution

Meet **Ice Beat-Le**, a symphatethic looking rover designed to combat ice by spreading salt, sand, or other suitable materials on your driveway or path. The rover is driven by six wheels and navigates using IR sensors ("eyes") to lock onto an IR beacon.

For the rover’s MCU, I selected the Particle Photon 2 because it's versatile, powerful, yet easy to work with. The IR beacon currently uses a Propeller Activity Board from Parallax because it includes an onboard D/A chip. If another Photon 2 had been available, I would likely have redesigned the beacon circuit and software to use Pulse Width Modulation (PWM) instead of the D/A chip.

![](/images/IMG-20250321-WA0001_compr.jpg)



### How does it work?

Although the rover looks complex due to its electronic components, motors, servo, and mechanical parts, it is actually quite straightforward, even minimalistic. There are no unnecessary bells and whistles - the rover does exactly what it’s intended to do, nothing more, nothing less. However, the journey of designing and building the rover involved overcoming numerous technical challenges.

#### Navigation

Navigation is achieved using an IR beacon transmitting a 38 kHz IR signal of varying intensity, utilizing the Sony protocol—the same used by many TV remote controls. Unfortunately, the IR receivers ("eyes") on the rover can only detect whether a modulated IR signal is present, not its intensity. This limitation makes it impossible to navigate using a constant IR signal alone, as the rover wouldn’t be able to determine if it's to the left or right of the beacon’s beam.

![](/images/Ice%20Beat-Le-1.png)
--*Rover on a driveway*--

  

To overcome this issue, I adopted a [solution](https://learn.parallax.com/courses/ir-beacon-seeking-activitybot-with-blocklyprop/) that involves varying the intensity of the IR signal. By modulating both the brightness and pulsing of the beacon, the rover measures how frequently it detects the IR signal, allowing it to determine its orientation relative to the beacon.

To generate both the modulated (pulsed) IR signal AND the varying brightness, two different signals are fed into the LED at the same time. The positive leg of the LED connects to an I/O pin, where the code is sending it pulses by turning it on and off 38000 times per second. The negative leg of the LED connects to a D/A-pin, which is changing its voltage output from 0V (full brightness for the LED) to 1.6V (nearly off). Why is 0V bright and 1.6V dim? Because this is where the negative leg is connected. The result is that the “Sawtooth”-like output of D/A is subtracted from the 3.3V pulses from the I/O pin:


![](/images/Ice%20Beat-Le-2.png)

The rover counts how often each IR sensor detects the beacon's light. If one side detects fewer pulses than the other (indicating that side is farther from the beacon), the rover turns in the opposite direction. E.g., in the driveway sketch further above the left sensor receives less light than the right, and thus the rover turns right. Since IR signals can be slightly erratic due to external interference, each receiver uses a moving average to stabilize readings.


![](/images/Ice%20Beat-Le-3.png)

![](/images/IR_beacon_case.png)

#### Sanding/Salting 

In the name of simplicity, and instead of inventing a salting mechanism needing a strong motor, I decided to use a force available to all of us for free → gravity. Sand, salt, or any other suitable material is poured into the container which has a hole in the bottom. To avoid that the sand falls away directly, a servo-controlled lid is opened or closed within the main control program. The sand falls down on the spreader which spreads it over approximately half a meter in width as the rover moves forward. If large particles are used, there's a risk of them clogging the container opening. However, as the rover doesn't have differential steering, it twitches a bit when alternating between driving, turning, and coasting, and I found out that this leads to that potential clogs are easily dissolved.


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
- separate power supply recommended for the motors, max 7.5 VDC; remember to connect grounds
- large breadboard
- 2 servo headers (for HB-25 motor controllers)
- Two 10kΩ resistors
- assorted wires


**IR Beacon**
- [Propeller Activity Board](https://www.parallax.com/product/propeller-activity-board-wx/), or any other microcontroller board with an D/A-chip. It is also possible to use PWM (Pulse Width Modulation) to replicate change in intensity.
- [TSAL6200 IR LED](https://www.partco.fi/en/leds/special-purpose-leds/17474-tsal6200.html), TSAL6100 even better as it has narrower angle
- [IRLZ44N Mosfet](https://www.partco.fi/en/electronic-components/actives/bjt-and-fet/switching-fets-n-channel/5057-irlz44n.html)
- 2.2Ω Resistor
- 10kΩ Resistor
- 5V power for the microcontroller, e.g. a powerbank
- assorted wires

**Note:** Ensure the program cycles the IR LED; constant operation may burn out the LED or resistor. A MOSFET is required to handle higher current beyond the MCU’s capability.


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


![](/images/Ice_Beat_Le.gif)



## Conclusion & Next Steps


### Issues overcome

As mentioned earlier, while the final result appears minimalistic, the journey to achieve it was neither simple nor straightforward. Indeed, there were several challenges and setbacks along the way. Finding a reliable solution for outdoor navigation took considerable experimentation - I had to acquire and test various IR LEDs and receivers extensively before finding an effective combination. Additionally, learning Fusion 360 from scratch, designing twelve distinct 3D-printed components, and printing numerous iterations consumed significant time and roughly one kilogram of filament, of which approximately 80% ended up as scrap.

### Future enhancements

This initial version of the rover isn't yet fully autonomous; for safety, it currently only moves when the IR beacon is transmitting. Presently, it moves forward and turns left or right as needed, but it would be relatively straightforward to add functionality enabling it to reverse its route or cover wider areas. Expanding the coverage could be achieved either by implementing a larger spreader mechanism or programming the rover to follow multiple parallel lanes.

To achieve full autonomy, adding bumper switches or distance sensors would allow obstacle avoidance, while integrating an odometer or a high-precision (centimeter-level) differential GPS would significantly enhance navigation capabilities—though these would add to the cost.

For permanent installations, the existing breadboard setup should be replaced by a custom PCB or at least a soldered protoboard. Additionally, the two separate battery systems currently in use could be consolidated into a single, higher-capacity battery for improved efficiency and runtime.


### Final Conclusions

This Ice Curing Rover, affectionately called Ice Beat-Le, is a six-wheeled autonomous vehicle that spreads sand or salt on icy driveways, helping reduce winter slip hazards. At the heart of the rover is the Particle Photon 2, whose powerful yet beginner-friendly platform made it easy to prototype, program, and control the robot’s complex behaviors with minimal overhead. Navigating via a modulated IR-beacon and using a gravity-fed dispenser controlled by a servo, the rover is a compact and cost-effective solution built with 3D-printed parts and off-the-shelf components. The Photon 2’s combination of real-time control, built-in connectivity, and versatility made it the ideal choice for this project - especially for balancing simplicity with performance. With a few additions, such as obstacle sensors or lane logic, the rover could easily evolve into a fully autonomous ice-clearing assistant.

While the *road* was bumpy to some *degree*, it *led* eventually to the goal, and it is now *ice* free! 

![](/images/Proud%20Beat-Le.png)

