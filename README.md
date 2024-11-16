Repository for the Bit-Bot robot. It is a bipedal walking robot, which likes to waddle around. :)

The robot leverages pre-programmed servo motions, as well as feedback from an IMU sensor, to achieve a walking gait.

Image:

![Screenshot_20240316_003534_Video Player](https://github.com/christianmueth/Bit-Bot/assets/59476460/d388fee1-2fcb-4230-b7a4-f7c8421df1db)

Circuit:
![image](https://github.com/user-attachments/assets/291f6f00-660a-4130-b077-f80c01456b87)


Code (see bit_bot4 ino file):

#include <ESP32Servo.h>

#include <Wire.h>

#include <Adafruit_MPU6050.h>

///lower gyro threshold on left foot, maybe raise on the right foot

///need to add in a PD loop

//need to be able to command forward gait and turns of left and right

//Uses ESP32 DEVKIT V1. Drivers for board are here:https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

Adafruit_MPU6050 mpu;//Uses an MPU6050 gyroscope

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

double del = 6;// 6 seems to work best -- good speed, good gait

void setup() {

  Serial.begin(115200);// declare serial baud rate // Initialize serial communication

  servo1.attach(SERVO_PIN1); // Attach servo1 to GPIO pin

  servo2.attach(SERVO_PIN2); // Attach servo2 to GPIO pin

  servo3.attach(SERVO_PIN3); // Attach servo3 to GPIO pin

  servo4.attach(SERVO_PIN4); // Attach servo4 to GPIO pin

  //Declare servo range

  //75 on servo 1 to 120 (left hip)

  //62 on servo 2 to 92 (right foot)

  //107 on servo 3 to 62 (right hip)

  //60 on servo 4 to 30 (left foot)

  servo1.write(75);

  servo2.write(62);

  servo3.write(107);

  servo4.write(90);

  // Initialize I2C communication with ESP32 SDA on GPIO 21, SCL on GPIO 22

  Wire.begin(21, 22);

  // Initialize MPU6050

  Serial.println("Initializing MPU6050...");

  if (!mpu.begin()) {

    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");

    while (1);

  }

  //Set the initial settings:

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);//184

}

void loop() {

  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  int left_foot_angle;

  int right_foot_angle;

  Serial.print(g.gyro.x);

  Serial.println();
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

  for (int angle = 62; angle <= 92; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo2.write(angle); // Set servo angle

  }

  state = 2;

}

//State 2: Move left foot.

if(state == 2)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 90; angle >= 50; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    if(g.gyro.x <= -2.95 && angle != 49)//try this at some point

    {

      left_foot_angle = angle;

      angle=49;

      break;

    }

    else if (angle > 49)

    {

      left_foot_angle = angle;

      servo4.write(angle); // Set servo angle

    }

  }

  state = 3;

}

//State 3 move right foot back

if(state == 3)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 92; angle >= 62; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo2.write(angle); // Set servo angle

  }

  state = 4;

}

//State 4 move left hip

if(state == 4)

{

  // Sweep the servo 45 degrees/ 

  for (int angle = 75; angle <= 120; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo1.write(angle); // Set servo angle

  }

  state = 5;

}

//State 5 move left foot back

if(state == 5)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = left_foot_angle; angle <= 90; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo4.write(angle); // Set servo angle

  }

  state = 6;

}

//State 6 move left hip back

if(state == 6)

{

  // Sweep the servo from 45 degrees/ 

  for (int angle = 120; angle >= 75; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo1.write(angle); // Set servo angle

  }

  state = 7;

}

//State 7 move left foot

if(state == 7)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 90; angle >= 60; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo4.write(angle); // Set servo angle

  }

  state = 8;

}

//State 8: Move right foot.

if(state == 8)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 62; angle <= 102; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    if(g.gyro.x >= 2.85 && angle != 103)

    {

      right_foot_angle = angle;

      angle=103;

      break;

    }

    else if (angle < 103)

    {

      right_foot_angle = angle;

      servo2.write(angle); // Set servo angle

    }

  }

  state = 9;

}
//State 9 move left foot back

if(state == 9)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 60; angle <= 90; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo4.write(angle); // Set servo angle

  }

  state = 10;

}

//State 10 move right hip

if(state == 10)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 107; angle >= 62; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo3.write(angle); // Set servo angle

  }

  state = 11;

}

//State 11 move right foot back

if(state == 11)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = right_foot_angle; angle >= 62; angle--) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo2.write(angle); // Set servo angle

  }

  state = 12;

}

//State 12 move right hip back

if(state == 12)

{

  // Sweep the servo from 0 to 180 degrees/ 

  for (int angle = 62; angle <= 107; angle++) {

    delay(del/2); // Delay for smoother movement

    mpu.getEvent(&a, &g, &temp);

    Serial.print(g.gyro.x);

    Serial.println();

    delay(del/2); // Delay for smoother movement

    servo3.write(angle); // Set servo angle

  }

  state = 1;

}

//Consider turning this whole shindig loop into a function, so that the code is not so messy

}
