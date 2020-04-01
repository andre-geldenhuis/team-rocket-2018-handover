#ifndef DATALOGGER_MAIN_H_
#define DATALOGGER_MAIN_H_

#include "globals.h"

// Range of different timer variables used throughout the system
extern uint32_t time1, time2, gps_timer, rf_timer, sd_write_timer, gimbal_delayer, launch_timer;

// The different states the rocket currently supports
enum State
{
	STARTUP,
	ARMED,
	PRELAUNCH,
	LAUNCH,
	POSTLAUNCH
};
extern State state;

// Generic sensor and sensor data objects. Made by us
#include "SensorObject/sensor.h"

#if GPS_ENABLED
#include "GPS/gps_driver.h"	
#include "GPS/TinyGPS++.h"	
extern TinyGPSPlus gps_lib;
extern GPS_Module gps;
#endif

#if IMU_ENABLED
#include "IMU/MPU9250.h"
#include "IMU/imu_driver.h"
extern MPU9250 imu_lib;
extern IMU_Module imu;
#endif

#if RF_ENABLED
#include <RH_RF95.h>
#include "RF/rf_driver.h"
extern RH_RF95 rf95;
extern RF_Module rf;
#endif

#if SD_ENABLED
#include "SD/sdcard_driver.h"
extern SD_Module sd;
#endif

#if CONTROL_ENABLED
#include "Control/control.h"
#endif

#define timer_rst 1000

//Stores the loop number
extern size_t loop_cnt;

void print_debug_info(void);
void next_state(void);

#endif
