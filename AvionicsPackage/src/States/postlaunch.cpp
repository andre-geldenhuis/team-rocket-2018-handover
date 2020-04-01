#include "postlaunch.h"
#if IS_REMOTE
static void handle_gps(void)
{
#if GPS_ENABLED
	gps.record();

#if DEBUG
	if (millis() > gps_timer + 10000)
	{
		gps_timer = millis();
		gps.print_status();
	}
#endif
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

static void handle_rf(void)
{
#if RF_ENABLED && GPS_ENABLED
    if (gps.get_tail() != NULL && millis() > rf_timer + 1000)
    {
		String ans;
		do
		{
			ans = rf.write(gps.get_tail()->to_string());		
			delay(100);
		} 
		while(ans != "ack");
		rf_timer = millis();	
    }
#endif
}

static void handle_sd(void)
{
#if SD_ENABLED
	
#if IMU_ENABLED
	if (imu.get_length() > LISTLENGTH) 
	{
		// debugger.print("sd>");	
		sd_write_timer = millis();
		String msg = imu.list_to_string();
		sd.write(SD_LOG_IMU, msg);
		// debugger.printf("IMU> SD write time: %d - %d\r\n", millis()-sd_write_timer, msg.length());
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

void postlaunch_loop(void)
{
	// debugger.println("Log:" + String(loop_cnt));
	handle_gps();
	handle_imu();
	handle_rf();
	handle_sd();
	center_gimbal();

#if DEBUG // Every 10 seconds prints the debug info
	if (millis() > time2 + 10000)
	{
		print_debug_info();
		time2 = millis();
	}
#endif

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