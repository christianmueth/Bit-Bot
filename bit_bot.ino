#include <ESP32Servo.h>
//Uses ESP32 DEVKIT V1. Drivers for board are here:https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

#define SERVO_PIN1 13 // GPIO pin connected to servo1
#define SERVO_PIN2 12 // GPIO pin connected to servo2
#define SERVO_PIN3 14 // GPIO pin connected to servo3
#define SERVO_PIN4 27 // GPIO pin connected to servo4
#define SERVO_MIN_PULSE_WIDTH       500     // the shortest pulse sent to a servo  
#define SERVO_MAX_PULSE_WIDTH      2500     // the longest pulse sent to a servo 
#define SERVO_MAX_ANGLE 180 // Maximum angle in degrees

Servo servo1; // Create servo objects
Servo servo2; // Create servo objects
Servo servo3; // Create servo objects
Servo servo4; // Create servo objects

int state = 0;//initialize state, to help with gait cycle
double del = 8.5;

void setup() {
  Serial.begin(115200);// declare serial baud rate // Initialize serial communication
  servo1.attach(SERVO_PIN1); // Attach servo1 to GPIO pin
  servo2.attach(SERVO_PIN2); // Attach servo2 to GPIO pin
  servo3.attach(SERVO_PIN3); // Attach servo3 to GPIO pin
  servo4.attach(SERVO_PIN4); // Attach servo4 to GPIO pin

  //Declare servo range
//80 on servo 1 to 125 (left hip)
//60 on servo 2 to 90 (right foot)
//100 on servo 3 to 55 (right hip)
//60 on servo 4 to 30 (left foot)
  servo1.write(80);
  servo2.write(60);
  servo3.write(100);
  servo4.write(60);
}

void loop() {
//State 0: Stance
if(state == 0)
{
  delay(5000);
  state = 1;
}
//State 1: Move right foot.
if(state == 1)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 57; angle <= 87; angle++) {
    servo2.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 2;
}

//State 2: Move left foot.
if(state == 2)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 60; angle >= 30; angle--) {
    servo4.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 3;
}

//State 3 move right foot back
if(state == 3)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 87; angle >= 57; angle--) {
    servo2.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 4;
}

//State 4 move left hip
if(state == 4)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 80; angle <= 125; angle++) {
    servo1.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 5;
}

//State 5 move left foot back
if(state == 5)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 30; angle <= 60; angle++) {
    servo4.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 6;
}

//State 6 move left hip back
if(state == 6)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 125; angle >= 80; angle--) {
    servo1.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 7;
}
//State 7 move left foot
if(state == 7)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 60; angle >= 30; angle--) {
    servo4.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 8;
}
//State 8: Move right foot.
if(state == 8)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 60; angle <= 90; angle++) {
    servo2.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 9;
}
//State 9 move left foot back
if(state == 9)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 30; angle <= 60; angle++) {
    servo4.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 10;
}
//State 10 move right hip
if(state == 10)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 100; angle >= 55; angle--) {
    servo3.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 11;
}
//State 11 move right foot back
if(state == 11)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 87; angle >= 57; angle--) {
    servo2.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 12;
}
//State 12 move right hip back
if(state == 12)
{
  // Sweep the servo from 0 to 180 degrees/ 
  for (int angle = 55; angle <= 100; angle++) {
    servo3.write(angle); // Set servo angle
    delay(del); // Delay for smoother movement
  }
state = 1;
}

//Consider turning this whole shindig loop into a function, so that the code is not so messy
}