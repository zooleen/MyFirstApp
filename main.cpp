/*
   -- wireless-stepper-stand --

   This source code of graphical user interface
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/
     - for ANDROID 4.3.1 or later version;
     - for iOS 1.3.5 or later version;

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/


#include <FlexyStepper.h>


//
// pin assignments
//
const int LED_PIN = 13;
const int MOTOR_STEP_PIN = 6;
const int MOTOR_DIRECTION_PIN  = 5 ;


//
// create the stepper motor object
//
FlexyStepper stepper;

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266_SOFTSERIAL_POINT
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
{ 255, 5, 0, 33, 0, 139, 0, 8, 13, 0,
  4, 128, 7, 14, 52, 5, 2, 26, 4, 128,
  7, 23, 52, 5, 2, 26, 4, 128, 3, 35,
  96, 8, 2, 26, 129, 0, 0, 10, 8, 3,
  17, 65, 99, 99, 101, 108, 0, 129, 0, 0,
  20, 9, 3, 17, 83, 112, 101, 101, 100, 0,
  129, 0, 2, 30, 12, 3, 17, 84, 97, 114,
  103, 101, 116, 32, 112, 111, 115, 105, 116, 105,
  111, 110, 0, 67, 4, 66, 13, 20, 5, 2,
  26, 11, 2, 0, 6, 53, 19, 5, 2, 26,
  31, 31, 84, 97, 107, 101, 0, 82, 101, 108,
  101, 97, 115, 101, 0, 67, 4, 66, 19, 20,
  5, 2, 26, 11, 67, 4, 62, 27, 32, 8,
  2, 26, 11, 1, 0, 78, 46, 12, 12, 2,
  31, 109, 111, 118, 101, 0
};

// this structure defines all the variables of your control interface
struct {

  // input variable
  int8_t accel_slider; // =0..100 slider position
  int8_t speed_slider; // =0..100 slider position
  int8_t position_slider; // =0..100 slider position
  uint8_t torque; // =1 if switch ON and =0 if OFF
  uint8_t button_1; // =1 if button pressed, else =0

  // output variable
  char current_accel[11];  // string UTF8 end zero
  char current_speed[11];  // string UTF8 end zero
  char current_target[11];  // string UTF8 end zero

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_TORQUE 7


void setup()
{
  RemoteXY_Init ();

  pinMode (PIN_TORQUE, OUTPUT);

  stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
  // TODO you setup code

}

void loop()
{
  RemoteXY_Handler ();

  digitalWrite(PIN_TORQUE, (RemoteXY.torque == 0) ? LOW : HIGH);

  uint8_t accel = map(RemoteXY.accel_slider, 0, 100, 1, 40);
  uint8_t speed = map(RemoteXY.speed_slider, 0, 100, 1, 80);
  uint16_t position = map(RemoteXY.position_slider, 0, 100, 0, 1140);


  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // RemoteXY.current_accel = RemoteXY.accel_slider();
  itoa(accel, RemoteXY.current_accel, 10);
  itoa(speed, RemoteXY.current_speed, 10);
  itoa(position, RemoteXY.current_target, 10);

  stepper.setStepsPerMillimeter(25 * 1);    // 1x microstepping

  stepper.setSpeedInMillimetersPerSecond(speed);
  stepper.setAccelerationInMillimetersPerSecondPerSecond(accel);

  if (RemoteXY.button_1) {
    RemoteXY.button_1 = 0;
    stepper.moveToPositionInMillimeters(position);
    delay(200);
  }

}
