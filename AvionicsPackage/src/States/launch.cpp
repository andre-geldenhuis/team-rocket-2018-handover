#include "launch.h"
#if IS_REMOTE
String control_vals = "";
size_t control_vals_cnt = 0;

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

#if CONTROL_ENABLED
	if (control_vals_cnt > LISTLENGTH)
	{
		sd_write_timer = millis();
		sd.write(SD_LOG_GIMBAL, control_vals);
		control_vals = "";
		control_vals_cnt = 0;
	}
#endif

#endif
}

static void handle_control(void)
{
#if CONTROL_ENABLED
	// uint32_t timer = millis();
	if ((millis() >= launch_timer + 1000) && (millis() < launch_timer + 1100))
	{
		//hardover
		control_vals = String(millis()) + "," + control_loop(true) + "\r\n";
		control_vals_cnt++;
	}
	else
	{
		//normal control
		control_vals = String(millis()) + "," + control_loop(false) + "\r\n";
		control_vals_cnt++;
		//debugger.printf("Control system time: %d\r\n", millis()-timer);
	}
#endif
}

void launch_check_state_change(void)
{
#if IMU_ENABLED
	/*
    IMU_Data* data = (IMU_Data*) imu.get_tail();
    if (data != NULL);
    {
        if(abs(data->accel_x) < POSTLAUNCH_THRES && abs(data->accel_y) < POSTLAUNCH_THRES && abs(data->accel_z < POSTLAUNCH_THRES))
        {
            next_state();
			rf_timer = millis();
        }
    }
	*/
	if (millis() > launch_timer + 1800)
	{
		next_state();
		rf_timer = millis();
	}
#endif
}

void launch_loop(void)
{
	// debugger.println("Log:" + String(loop_cnt));
	handle_gps();
	handle_imu();
	handle_sd();
	handle_control();

	launch_check_state_change();

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