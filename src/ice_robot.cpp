/* 
 * Project Ice Beat-Le
 * Author: Thomas Vikström
 * Date: 21.3.2025
 * Description: This program is designed to control a robot with two motors and two IR sensors.
 * 
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <cstdlib> // Include the necessary library for random function


// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

Servo left;                                       // create servo object to control a servo
Servo right;                                      // create servo object to control a servo
Servo lid;                                        // create servo object to control a servo

const int LEFT_MOTOR_PIN = D15; 
const int RIGHT_MOTOR_PIN = D1;
const int LEFT_IR_PIN = D2;
const int RIGHT_IR_PIN = D10;
const int LID_PIN = D16;


// ------ Functions ------

void forward(int speed, int time) {
  left.write(90 + speed);   // Move left motor forward
  right.write(90 - speed);  // Move right motor forward
  delay(time);              // Delay selected time
}

void drive(int left_speed, int right_speed, int time) {
  left.write(90 + left_speed);    // Move left motor forward
  right.write(90 - right_speed);  // Move right motor forward
  delay(time);                    // Delay selected time
}

void stop(int time) {
  left.write(90);         // Stop both motors
  right.write(90);   
  delay(time);            // Delay selected time 
}


// Function to calculate the rolling average for a given sensor
float calculateRollingAverage(float newSample, int sensorID) {
  static int amount = 3;
  static float samples[2][3] = {{0}}; // Arrays to store the last 10 samples for two sensors
  static int index[2] = {0};          // Current index for each sensor
  static int count[2] = {0};          // Number of samples added (up to 10) for each sensor
  static float sum[2] = {0};          // Sum of the samples for each sensor

  // Subtract the oldest sample from the sum (if array is full)
  sum[sensorID] -= samples[sensorID][index[sensorID]];

  // Add the new sample to the array and update the sum
  samples[sensorID][index[sensorID]] = newSample;
  sum[sensorID] += newSample;

  // Update the index for the sensor
  index[sensorID] = (index[sensorID] + 1) % amount;

  // Increment count up to 10 for the sensor
  if (count[sensorID] < amount) {
      count[sensorID]++;
  }

  // Return the rolling average for the sensor
  return sum[sensorID] / count[sensorID];
}


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


// --------------------------------------------------------------------------
// Open and close the lid
// --------------------------------------------------------------------------
void move_lid() {      

  int speed = 5;
  int wait = 15;
  int servo_pos = 0;              // variable to store the servo position


  for (servo_pos = 170; servo_pos >= 1; servo_pos -= speed) {
      lid.write(servo_pos);
      delay(wait);
  }

  delay(wait);

  for (servo_pos = 0; servo_pos < 170; servo_pos += speed) {   
    lid.write(servo_pos);
    delay(wait);
  }

}



void setup() {


  // Set up the IR sensors
  pinMode(LEFT_IR_PIN, INPUT);                               // sets the digital pin as input
  pinMode(RIGHT_IR_PIN, INPUT);                              // sets the digital pin as input

  // Set up the motors
  left.attach(LEFT_MOTOR_PIN);                               // attaches the left servo  to the servo object
  right.attach(RIGHT_MOTOR_PIN);                             // attaches the right servo to the servo object
  lid.attach(LID_PIN);                                       // attaches the lid servo to the servo object
}


void loop() {
  int leftIR = 0;
  int rightIR = 0;
  
  IR_read(leftIR, rightIR);                                 // Read the IR sensors  

  int leftSpeed = 23;                                       // Set the speed for the left motor (the left motor is slower)
  int rightSpeed = 20;                                      // Set the speed for the right motor
  int drive_time = 100;                                     // Set the drive time        
  int coeff = 3;                                            // Set the coefficient for the difference between the IR sensors  

  int left_avg = calculateRollingAverage(leftIR, 0);        // Rolling average for Sensor 1
  int right_avg = calculateRollingAverage(rightIR, 1);      // Rolling average for Sensor 2
  int diff = coeff * (left_avg - right_avg);                // Calculate the difference between the IR sensors

  Log.info("   Left avg: %2d,   Right avg: %2d,   Diff: %3d", left_avg, right_avg, left_avg - right_avg);
  

  move_lid();                                                // Open and close the lid

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
}
