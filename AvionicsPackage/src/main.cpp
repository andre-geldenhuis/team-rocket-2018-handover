#include "main.h"

// Debugger object used as a wrapper for all serial output, externed in `globals.h`
Debugger debugger;

#if IS_REMOTE
#include "States/startup.h"
#include "States/armed.h"
#include "States/prelaunch.h"
#include "States/launch.h"
#include "States/postlaunch.h"
#endif

#if GPS_ENABLED
TinyGPSPlus gps_lib; 		// The external GPS library
GPS_Module gps(&gps_lib);	// Our GPS library implementation with the external library being passed through
#endif

#if IMU_ENABLED
MPU9250 imu_lib(SPI, IMU_CS);	// The external IMU library, with the IMU on the SPI0 bus connected to cs pin 15
IMU_Module imu(&imu_lib);		// Our IMU library implementation with the external library being passed through
#endif

#if RF_ENABLED
RH_RF95 rf95(RFM95_CS, RFM95_INT);	// The external RF library, with the RF Module on the SPI0 bus connected to cs pin 10 and int pin 9
RF_Module rf(&rf95);				// Our RF library implementation with the external library being passed through
#endif

#if SD_ENABLED
SD_Module sd;	// Our SD card library declaration
#endif

// Declaration for the rockets state, extered in `main.h`
State state;

// For recording time performance
uint32_t time1, time2, gps_timer, rf_timer, sd_write_timer, gimbal_delayer, launch_timer;

size_t loop_cnt;

/**
 * Simply outputs the number of times each module has been written/read 
 */
void print_debug_info(void)
{
	debugger.printf("\r\nLoop Count: %d\r\n", loop_cnt);
#if GPS_ENABLED
	debugger.printf("GPS Reads: %d\r\n", gps.reads);
#endif
#if IMU_ENABLED
	debugger.printf("IMU Reads: %d\r\n", imu.reads);
#endif
#if SD_ENABLED
	debugger.printf("SD Card Writes: %d\r\n", sd.writes);
#endif
#if RF_ENABLED
	debugger.printf("RF Broadcasts: %d\r\n\r", rf.writes);
#endif
}

/**
 * Enables the rocket to change between states.
 * On each state change, the time and current states 
 * get logged to the SD card as well as force broadcasted
 * over rf. 
 * 
 * Forced rf broadcasting is when we keep broadcasting until 
 * we receive an acknowledgement
 * 
 */
#if IS_REMOTE
void next_state(void)
{
	String ans;
	switch (state)
	{
	case STARTUP: 	/*******************************************************************/
		state = ARMED;
		
		debugger.println(String(millis()) + ", Changing state from STARTUP to ARMED");

#if SD_ENABLED
		sd.write(SD_LOG_DEBUG, String(millis()) + ", Changing state from STARTUP to ARMED");
#endif

#if RF_ENABLED
		do
		{
			ans = rf.write("Changing state from STARTUP to ARMED");
			delay(200);
		} 
		while(ans != "ack");
#endif
		
		break;

	case ARMED: 	/*******************************************************************/
		state = PRELAUNCH;
		
		debugger.println(String(millis()) + ", Changing state from ARMED to PRELAUNCH");

#if SD_ENABLED
		sd.write(SD_LOG_DEBUG, String(millis()) + ", Changing state from ARMED to PRELAUNCH");
#endif

#if RF_ENABLED
		do
		{
			ans = rf.write("Changing state from ARMED to PRELAUNCH");
			delay(200);
		} 
		while(ans != "ack");
#endif
		
		break;
	
	case PRELAUNCH:	/*******************************************************************/
		state = LAUNCH;
		launch_timer = millis(); // Timer starts for when to exit the launch state
		
		debugger.println(String(launch_timer) + ", Changing state from PRELAUNCH to LAUNCH");

#if SD_ENABLED
		sd.write(SD_LOG_DEBUG, String(launch_timer) + ", Changing state from PRELAUNCH to LAUNCH");
#endif

		// This state has no rf broadcast because it takes too long and it needs to start working on control code

		break;
	case LAUNCH:	/*******************************************************************/
		state = POSTLAUNCH;
		
		debugger.println(String(millis()) + ", Changing state from LAUNCH to POSTLAUNCH");

#if SD_ENABLED
		sd.write(SD_LOG_DEBUG, String(millis()) + ", Changing state from LAUNCH to POSTLAUNCH");
#endif

#if RF_ENABLED
		do
		{
			ans = rf.write("Changing state from LAUNCH to POSTLAUNCH");
			delay(200);
		}
		while(ans != "ack");		
#endif

		break;
	case POSTLAUNCH:
		break; // No state after postlaunch
	}
}
#endif

/**
 * The looping method fot the arduino project
 * This method is run after setup() has completed
 *  and will continue to run until the program is stopped
 */
void loop(void)
{
	loop_cnt++;
	// debugger.println(String(loop_cnt));
#if IS_REMOTE
	switch (state)
	{
	case STARTUP:
		startup_loop();
		break;
	case PRELAUNCH:
		prelaunch_loop();
		break;
	case ARMED:
		armed_loop();
		break;
	case LAUNCH:
		launch_loop();
		break;
	case POSTLAUNCH:
		postlaunch_loop();
		break;
	};
#else
	String msg = rf.read();
	if (msg != "") // Replies to every received message with an ack
	{
		Serial.println(">>" + msg);
		rf.write("ack");
	}
#endif
	// delay(10);
}

/** 	
 * The setup method for the arduino project
 * This is the first method run and is responsible
 * 	for setting up each module used.
 * 
 * This will only be run once on startup.
 */
void setup(void)
{
	debugger.init();
	debugger.println("Setup started");

	pinMode(LED_BUILTIN,OUTPUT);
	digitalWrite(LED_BUILTIN,HIGH);

	state = STARTUP;

	bool setup_success = true;

#if IS_REMOTE
#if GPS_ENABLED
	setup_success &= gps.init();
#endif

#if IMU_ENABLED
	setup_success &= imu.init();
#endif

#if RF_ENABLED
	setup_success &= rf.init();
	rf.write("Ini board started - RF success");
#endif

#if SD_ENABLED
	setup_success &= sd.init();
#endif

#if CONTROL_ENABLED
	control_setup(&imu_lib);
#endif
	gimbal_setup();
#else
	setup_success &= rf.init();
#endif

	// Checks if all enabled components have initialised
	// If not, doesn't allow the loop to be run
	if (setup_success)
	{
		debugger.println("Setup successful");
#if RF_ENABLED
		rf.write("Board setup successful");
#endif
	}
	else
	{
		debugger.println("Setup failed");
#if RF_ENABLED
		rf.write("Board setup failed");
#endif
		while (1)
			;
	}
	time1 = time2 = gps_timer = rf_timer = gimbal_delayer = millis();
	// while(1);
}