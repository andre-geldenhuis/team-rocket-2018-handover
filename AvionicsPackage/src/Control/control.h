#ifndef DATALOGGER_CONTROL_H_
#define DATALOGGER_CONTROL_H_

#include "../main.h"
#include "../IMU/imu_driver.h"
#include <arm_math.h>
#include "quaternion.h"

void control_setup(MPU9250* imu_obj);
void control_setup(void);
String control_loop(bool hardover);

extern arm_matrix_instance_f32 aCounts, gCounts, hCounts;
extern arm_matrix_instance_f32 aVals, gVals, hVals;
extern arm_matrix_instance_f32 aCtoV, gCtoV, hCtoV;

extern float32_t aCountsf[4], gCountsf[4], hCountsf[4];
extern float32_t aValsf[4], gValsf[4], hValsf[4];
extern float32_t aCtoVf[16], gCtoVf[16], hCtoVf[16];

// Gyro Value to Quaternion;
extern arm_matrix_instance_f32 gVtoQ;
extern float32_t gVtoQf[16];

extern arm_pid_instance_f32 yawController, pitchController;

#endif