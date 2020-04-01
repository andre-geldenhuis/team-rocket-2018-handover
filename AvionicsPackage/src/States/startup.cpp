#include "startup.h"
#if IS_REMOTE

// Each value is set to true as they are confirmed to be functioning.
// Once they are proved to be functioning, they no longer get checked
bool imu_valid = false, gps_valid = false, sd_valid = false, rf_valid = false;

static bool handle_gps(void)
{
#if GPS_ENABLED
	if (!gps_valid)
	{
		GPS_Data *data = (GPS_Data *)gps.read();
		if (data != NULL)
		{
			delete data;
			gps_valid = gps_lib.sentencesWithFix() > 0;

			if (gps_valid)
			{
				debugger.println("GPS Fix Found");
				return true;
			}
			else
			{
				debugger.println("No GPS Fix");
#if RF_ENABLED
				if (millis() > gps_timer + 10000)
				{
					gps_timer = millis();
					rf.write(String(gps_timer) + ": No GPS fix");
				}
#endif
				return false;
			}
		}
		return false;
		// return true; //Usefull for debugging
	}
#endif
	return true;
}

static bool handle_imu(void)
{
#if IMU_ENABLED
	if (!imu_valid)
	{
		IMU_Data *dat = imu.read();
		imu_valid = dat->accel_x && dat->accel_y && dat->accel_z && dat->gyro_x && dat->gyro_y && dat->gyro_z && dat->magn_x && dat->magn_y && dat->magn_z;
		delete dat;
		if (imu_valid)
		{
			debugger.println("Valid IMU Data");
			return true;
		}
		else
		{
			debugger.println("Invalid IMU Data");
			return false;
		}
	}
#endif
	return true;
}

static bool handle_sd(void)
{
#if SD_ENABLED
	if (!sd_valid)
	{
		sd_valid = sd.write(SD_LOG_DEBUG, String(millis()) + ", SD writing is valid");
		if (sd_valid)
		{
			debugger.println("SD card printing successfully");
			return true;
		}
		else
		{
			debugger.println("SD card printing failed");
			return false;
		}
	}
#endif
	return true;
}

static bool handle_rf(void)
{
#if RF_ENABLED
	if (!rf_valid)
	{
		// Keeps checking until an ack is reveived
		String resp = rf.write("RFSANITY");
		rf_valid = resp.equals("ack");
		// delay(200);
		if (rf_valid)
		{
			debugger.println("RF communication success");
			return true;
		}
		else
		{
			debugger.println("RF communication failed");
			return false;
		}
	}
#endif
	return true;
}

void startup_loop(void)
{
	bool startup_success = true;

	startup_success &= handle_gps();
	startup_success &= handle_imu();
	startup_success &= handle_sd();
	startup_success &= handle_rf();

	// If all modules are functioning, move onto next state
	if (startup_success)
	{
#if SD_ENABLED
		sd.write(SD_LOG_DEBUG, String(millis()) + ", All sensors working");
#endif
#if RF_ENABLED
		rf.write("All components working");
#endif
		next_state();
	}
}
#endif