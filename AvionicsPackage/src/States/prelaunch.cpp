#include "prelaunch.h"
#if IS_REMOTE
static void handle_gps(void)
{
#if GPS_ENABLED
	gps.record();

	if (millis() > gps_timer + 10000)
	{
		gps_timer = millis();
		gps.print_status();
	}
#endif
}

static void handle_imu(void)
{
#if IMU_ENABLED
	if (imu_lib.checkDataReadyInterrupt())
	{
		imu.record();
	}
#endif
}

static void handle_sd(void)
{
#if SD_ENABLED
	
#if IMU_ENABLED
	if (imu.get_length() > LISTLENGTH) 
	{
		sd_write_timer = millis();
		String msg = imu.list_to_string();
		sd.write(SD_LOG_IMU, msg);
		debugger.println(msg);
	}
#endif

#if GPS_ENABLED
	if (gps.get_length() > LISTLENGTH)
	{
		// debugger.print("sd>");	
		sd_write_timer = millis();
		String message = gps.list_to_string();
		sd.write(SD_LOG_GPS, message);
		// debugger.printf("GPS> SD write time: %d - %d\r\n", millis()-sd_write_timer, message.length());	
	}
#endif

#endif
}

static void check_state_change(void)
{
#if IMU_ENABLED
    IMU_Data* data = (IMU_Data*) imu.get_tail();
    if (data != NULL);
    {
		// If any accel axis go over the launch threshold, assume rocket has launched
        if(abs(data->accel_x) > LAUNCH_THRES || abs(data->accel_y) > LAUNCH_THRES || abs(data->accel_z > LAUNCH_THRES))
        {
            next_state();
        }
    }
#endif
}   

void prelaunch_loop(void)
{
	// debugger.println("Log:" + String(loop_cnt));
	handle_gps();
	handle_imu();
	handle_sd();

    check_state_change();

#if IMU_ENABLED
	if (imu.get_length() > LISTLENGTH)
	{
		imu.clear_list();
	}
#endif

#if GPS_ENABLED
	if (gps.get_length() > LISTLENGTH)
	{
		gps.clear_list();
	}
#endif
}
#endif