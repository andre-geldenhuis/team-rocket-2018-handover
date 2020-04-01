/**
 * imu_driver.cpp
 * The driver for the MPU9250 IMU used in the avionics package
 * Handles the datalist as well as reading from and managing the imu
 */

#include "imu_driver.h"

SPISettings imu_spi_settings(20000000, MSBFIRST, SPI_MODE3);

IMU_Module::IMU_Module(MPU9250* libObj)
{
	imu_device = libObj;
}

bool IMU_Module::init(void)
{
	SPI.setSCK(SPI_SCK);
	debugger.println("IMU Init Started");
		
	int status = imu_device->begin();

	if (status < 0) {
		debugger.printf(">> Error Code: %d\r\n", status);
		debugger.println(">> IMU Init Failed");
		return false;
	}

	// setting the accelerometer full scale range to +/-16G 
  	imu_device->setAccelRange(MPU9250::ACCEL_RANGE_16G);
  	// setting the gyroscope full scale range to +/-2000 deg/s
  	imu_device->setGyroRange(MPU9250::GYRO_RANGE_2000DPS);	

	//184hz
	imu_device->setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);
	imu_device->setSrd(0);

	imu_device->enableDataReadyInterrupt();

	debugger.println("IMU Init Success");

	return true;
}

IMU_Data* IMU_Module::read(void)
{
	reads++;
	SPI.beginTransaction(imu_spi_settings);
	SPI.transfer(0);
	SPI.endTransaction();

	imu_device->readSensor();

	IMU_Data* data = new IMU_Data();

	data->accel_x = imu_device->getAccelX_mss();
	data->accel_y = imu_device->getAccelY_mss();
	data->accel_z = imu_device->getAccelZ_mss();
	data->gyro_x = imu_device->getGyroX_rads();
	data->gyro_y = imu_device->getGyroY_rads();
	data->gyro_z = imu_device->getGyroZ_rads();
	data->magn_x = imu_device->getMagX_uT();
	data->magn_y = imu_device->getMagY_uT();
	data->magn_z = imu_device->getMagZ_uT();

	return data;
}

String IMU_Data::to_string(void)
{
	// debugger.println("imu>to_string");	
	String accel = String(accel_x, 6)+","+String(accel_y, 6)+","+String(accel_z, 6);
	String magn = String(magn_x, 6)+","+String(magn_y, 6)+","+String(magn_z, 6);
	String gyro = String(gyro_x, 6)+","+String(gyro_y, 6)+","+String(gyro_z, 6);
	// debugger.println("to_string_finished");	
	return String(timestamp) + "," + accel + "," + gyro + "," + magn;
}
