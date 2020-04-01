#ifndef DATALOGGER_GLOBALS_
#define DATALOGGER_GLOBALS_

#include "Arduino.h"

#include "Debugger/debugger.h"
extern Debugger debugger;
#define DEBUG false

#define IS_REMOTE true

#if IS_REMOTE
#define GPS_ENABLED true
#define IMU_ENABLED true
#define SD_ENABLED true
#define RF_ENABLED true
#define CONTROL_ENABLED true
#else
#define RF_ENABLED true
#endif

#define SPI_SCK 27
#define SERIAL_BAUD_RATE 9600
#define LISTLENGTH 22

#define SD_LOG_DEBUG "debuglog.txt"
#define SD_LOG_IMU "imudata.txt"
#define SD_LOG_GPS "gpsdata.txt"
#define SD_LOG_GIMBAL "gimdata.txt"

#endif