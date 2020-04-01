#include "control.h"
#include "../Gimbal/gimbal_driver.h"

arm_matrix_instance_f32 aCounts, gCounts, hCounts;
arm_matrix_instance_f32 aVals, gVals, hVals;
arm_matrix_instance_f32 aCtoV, gCtoV, hCtoV;

float32_t aCountsf[4], gCountsf[4], hCountsf[4];
float32_t aValsf[4], gValsf[4], hValsf[4];
float32_t aCtoVf[16], gCtoVf[16], hCtoVf[16];

Quaternion absRot;
Quaternion instQuat;

arm_pid_instance_f32 yawController, pitchController;

#define KP 1
#define KD 1
#define KI 1

// an MPU9250 object with the MPU-9250 sensor on SPI bus 0 and chip select pin 10
MPU9250* imu_device;
int status;
int count = 0;

String matStr(arm_matrix_instance_f32 mat) 
{
    String s = "[" ;
    for(int i=0; i<mat.numRows; i++) {
        for(int j=0; j<mat.numCols; j++) {
            s.append(String(mat.pData[i*mat.numCols + j], 10));
            if(j+1<mat.numCols) {
                s.append(", ");
            }
        }
        if(i+1<mat.numRows) {
            s.append("\n");
        }
    }
    s.append("]");
    return s;
}

void linkLoopMats(void) {
    arm_mat_init_f32(&aCounts, 4, 1, aCountsf);
    arm_mat_init_f32(&gCounts, 4, 1, gCountsf);
    arm_mat_init_f32(&hCounts, 4, 1, hCountsf);

    arm_mat_init_f32(&aVals, 4, 1, aValsf);
    arm_mat_init_f32(&gVals, 4, 1, gValsf);
    arm_mat_init_f32(&hVals, 4, 1, hValsf);
    
    arm_mat_init_f32(&aCtoV, 4, 4, aCtoVf);
    arm_mat_init_f32(&gCtoV, 4, 4, gCtoVf);
    arm_mat_init_f32(&hCtoV, 4, 4, hCtoVf);

    // absRot* = new Quaternion();
}

void initMats(void) {
    arm_matrix_instance_f32 _aCtoV, __aCtoV, ___aCtoV;      float32_t _aCtoVf[16], __aCtoVf[16], ___aCtoVf[16];
    arm_matrix_instance_f32 _gCtoV, __gCtoV, ___gCtoV;      float32_t _gCtoVf[16], __gCtoVf[16], ___gCtoVf[16];
    arm_matrix_instance_f32 _hCtoV, __hCtoV, ___hCtoV;      float32_t _hCtoVf[16], __hCtoVf[16], ___hCtoVf[16];
    arm_mat_init_f32(&_aCtoV, 4, 4, _aCtoVf);   arm_mat_init_f32(&__aCtoV, 4, 4, __aCtoVf);   arm_mat_init_f32(&___aCtoV, 4, 4, ___aCtoVf);
    arm_mat_init_f32(&_gCtoV, 4, 4, _gCtoVf);   arm_mat_init_f32(&__gCtoV, 4, 4, __gCtoVf);   arm_mat_init_f32(&___gCtoV, 4, 4, ___gCtoVf);
    arm_mat_init_f32(&_hCtoV, 4, 4, _hCtoVf);   arm_mat_init_f32(&__hCtoV, 4, 4, __hCtoVf);   arm_mat_init_f32(&___hCtoV, 4, 4, ___hCtoVf);

    float32_t abx = -imu_device->_axb, aby = -imu_device->_ayb, abz = -imu_device->_azb;    float32_t gbx = -imu_device->_gxb, gby = -imu_device->_gyb, gbz = -imu_device->_gzb;    float32_t hbx = -imu_device->_hxb, hby = -imu_device->_hyb, hbz = -imu_device->_hzb;
    float32_t asx = imu_device->_accelScale, asy = imu_device->_accelScale, asz = imu_device->_accelScale;    float32_t gsx = imu_device->_gyroScale, gsy = imu_device->_gyroScale, gsz = imu_device->_gyroScale;    float32_t hsx = imu_device->_magScaleX, hsy = imu_device->_magScaleY, hsz = imu_device->_magScaleZ;

    // float32_t abx = 0, aby = 0, abz = 0;    float32_t gbx = 0.0097779445, gby = 0.0037884042, gbz = -0.0111122085;    float32_t hbx = 0, hby = 0, hbz = 0;
    // float32_t asx = 0.0047886474, asy = 0.0047886474, asz = 0.0047886474;    float32_t gsx = 0.0001331601, gsy = 0.0001331601, gsz = 0.0001331601;    float32_t hsx = 0.1786382049, hsy = 0.1786382049, hsz = 0.1716098189;

    arm_matrix_instance_f32 _aBias;     float32_t _aBiasf[16] = {
        1,  0,  0,  abx,
        0,  1,  0,  aby,
        0,  0,  1,  abz,
        0,  0,  0,  1   };
    arm_matrix_instance_f32 _gBias;     float32_t _gBiasf[16] = {
        1,  0,  0,  gbx,
        0,  1,  0,  gby,
        0,  0,  1,  gbz,
        0,  0,  0,  1   };
    arm_matrix_instance_f32 _hBias;     float32_t _hBiasf[16] = {
        1,  0,  0,  hbx,
        0,  1,  0,  hby,
        0,  0,  1,  hbz,
        0,  0,  0,  1   };

    
    arm_matrix_instance_f32 _aScale;    float32_t _aScalef[16] = {
        asx,0,  0,  0,
        0,  asy,0,  0,
        0,  0,  asz,0,
        0,  0,  0,  1   };
        
    arm_matrix_instance_f32 _gScale;    float32_t _gScalef[16] = {
        gsx,0,  0,  0,
        0,  gsy,0,  0,
        0,  0,  gsz,0,
        0,  0,  0,  1   };
    arm_matrix_instance_f32 _hScale;    float32_t _hScalef[16] = {
        hsx,0,  0,  0,
        0,  hsy,0,  0,
        0,  0,  hsz,0,
        0,  0,  0,  1   };


    arm_matrix_instance_f32 _aRotation; float32_t _aRotationf[16] = {
        0,  1,  0,  0,
        1,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  1,  };
    arm_matrix_instance_f32 _gRotation; float32_t _gRotationf[16] = {
        0,  1,  0,  0,
        1,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  1,  };
    arm_matrix_instance_f32 _hRotation; float32_t _hRotationf[16] = {
        1,  0,  0,  0,
        0,  1,  0,  0,
        0,  0,  1,  0,
        0,  0,  0,  1   };


    arm_mat_init_f32(&_aBias, 4, 4, _aBiasf);
    arm_mat_init_f32(&_gBias, 4, 4, _gBiasf);
    arm_mat_init_f32(&_hBias, 4, 4, _hBiasf);
    arm_mat_init_f32(&_aScale, 4, 4, _aScalef);
    arm_mat_init_f32(&_gScale, 4, 4, _gScalef);
    arm_mat_init_f32(&_hScale, 4, 4, _hScalef);
    arm_mat_init_f32(&_aRotation, 4, 4, _aRotationf);
    arm_mat_init_f32(&_gRotation, 4, 4, _gRotationf);
    arm_mat_init_f32(&_hRotation, 4, 4, _hRotationf);
    
    //NB gBias and gScale are applied in opposite order to aScale and aBias because of the way the 
    //library i'm mimmicking calculates its biases and scales

    arm_mat_mult_f32(&_aScale, &_aBias, &_aCtoV);
    arm_mat_mult_f32(&_gBias, &_gScale, &_gCtoV);
    arm_mat_mult_f32(&_hScale, &_hBias, &_hCtoV);

    arm_mat_mult_f32(&_aCtoV, &_aRotation, &aCtoV);
    arm_mat_mult_f32(&_gCtoV, &_gRotation, &gCtoV);
    arm_mat_mult_f32(&_hCtoV, &_hRotation, &hCtoV);
}



void _control_setup(void)
{
//   printParams();
//   Serial.println("hello");
    linkLoopMats();
    initMats();

    yawController.Kp = KP;
    yawController.Ki = KI;
    yawController.Kd = KD;
    pitchController.Kp = KP;
    pitchController.Ki = KI;
    pitchController.Kd = KD;
    arm_pid_init_f32(&yawController, 1);
    arm_pid_init_f32(&pitchController, 1);

    
//   Serial.println(matStr(gCtoV));
//   Serial.println("finnished");
//   initTestData();

//   float magRot = arm
}

void control_setup(MPU9250* libObj) {
    imu_device = libObj;
    _control_setup();
}

void control_setup(void) {
    _control_setup();
}

uint32_t gimbal_time = 0;
String control_loop(bool hardover=false) {  

// recording for thingy
    // debugger.println(">" + String(gimbal_time, 10));

    float diff = (micros() - (gimbal_time > 0 ? gimbal_time : micros()))/1000000.0;
    // float diff = 0.001;
    imu_device->readSensor();
    gimbal_time = micros();

    aCountsf[0] = imu_device->_axcounts;
    aCountsf[1] = imu_device->_aycounts;
    aCountsf[2] = imu_device->_azcounts;
    aCountsf[3] = 1;
    gCountsf[0] = imu_device->_gxcounts;
    gCountsf[1] = imu_device->_gycounts;
    gCountsf[2] = imu_device->_gzcounts;
    gCountsf[3] = 1;
    hCountsf[0] = imu_device->_hxcounts;
    hCountsf[1] = imu_device->_hycounts;
    hCountsf[2] = imu_device->_hzcounts;
    hCountsf[3] = 1;

    arm_mat_mult_f32(&aCtoV, &aCounts, &aVals);
    arm_mat_mult_f32(&gCtoV, &gCounts, &gVals);
    arm_mat_mult_f32(&hCtoV, &hCounts, &hVals);

    instQuat.from_euler(imu_device->_d2r*gValsf[0]*diff, imu_device->_d2r*gValsf[1]*diff, imu_device->_d2r*gValsf[2]*diff);

    absRot.quaternionProduct(&instQuat);
    absRot.normalise();
    float roll, pitch, yaw;
    // absRot.to_euler(&roll, &pitch, &yaw);
    absRot.to_euler(&yaw, &pitch, &roll);
    // debugger.println(String(gValsf[0], 10) + ", " + String(gValsf[1], 10) + ", " + String(gValsf[2], 10));

    float gimbalYaw = arm_pid_f32(&yawController, yaw);
    float gimbalPitch = arm_pid_f32(&pitchController, pitch);

    if (hardover == true){
        gimbalPitch=10; //hard over max pitch
        debugger.println("Pitch hardover");
    }

	if (millis() > gimbal_delayer + 10)
	{
        gimbal_loop(gimbalYaw, gimbalPitch);
        debugger.println("Pitch: " + String(gimbalPitch, 10) + "\r\nYaw: " + String(gimbalYaw, 10));
        gimbal_delayer = millis();
        // millis();  
        // debugger.println(">>" + String(diff, 10));
        // debugger.println(">>>" + String(gimbal_time, 10));

    }

    return String(diff, 10) + "" + String(roll, 10) + "," + String(pitch, 10) + "," + String(yaw, 10);
}