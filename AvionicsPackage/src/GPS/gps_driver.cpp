#include "gps_driver.h"

#define gpsSerial Serial4
#define gpsSerialBaud 4800 // For A220
// #define gpsSerialBaud 9600	// For SparkFun Venus
#define GPS_ON_OFF 24
#define RXPIN 31 // Receive pin
#define TXPIN 32 // Transmit pin

#define TOP_ANT 33 // Top antenna
#define BOT_ANT 34 // Bottom antenna

GPS_Module::GPS_Module(TinyGPSPlus* libObj)
{
	gps_device = libObj;
}

bool GPS_Module::init(void)
{
	debugger.println("GPS Init Started");

	// activates the top antenna on the system
	pinMode(TOP_ANT, OUTPUT);
	pinMode(BOT_ANT, OUTPUT);

	// turns on antenna
	digitalWrite(BOT_ANT, LOW);
	digitalWrite(TOP_ANT, HIGH);

	//sets up the activate pulse for the system
	pinMode(GPS_ON_OFF, OUTPUT);
	pinMode(TXPIN, OUTPUT);
	pinMode(RXPIN, INPUT);

	delay(1000);

	if (check_start() || check_start())
	{
		debugger.println("GPS Init Success");
		return true;
	}

	debugger.println(">> GPS Init Failed");
	return false;
}

bool GPS_Module::check_start(void)
{
	digitalWrite(GPS_ON_OFF, HIGH);
	digitalWrite(TXPIN, HIGH);
	delay(200);
	digitalWrite(GPS_ON_OFF, LOW);
	digitalWrite(TXPIN, LOW);

	gpsSerial.begin(gpsSerialBaud);

	uint32_t start_time = millis();
	while (millis() < start_time + 5000)
	{
		if (gpsSerial.available() > 0)
		{
			int status = gpsSerial.read();
			gps_device->encode(status);
		}

		if (gps_device->charsProcessed() > 10)
		{
			return true;
		}
	}
	return false;
}

GPS_Data *GPS_Module::read(void)
{
	// Swaps the antenna to use
	digitalWrite(BOT_ANT, useTop ? LOW : HIGH);
	digitalWrite(TOP_ANT, useTop ? HIGH : LOW);
	useTop = !useTop;

	// Checks and reads from the GPS module
	while (gpsSerial.available() > 0)
	{
		int status = gpsSerial.read();
		// debugger.printf("%c", status);
		if (gps_device->encode(status))
		{ //loads the stream of data to the tinyGPS library
			reads++;
			GPS_Data* data = new GPS_Data();
			data->time = gps_device->time;
			data->location = gps_device->location;
			return data;
		}
		// delay(1);
	}

	return NULL;
}

void GPS_Module::print_status(void)
{
	debugger.println("Chars Processed: " + String(gps_device->charsProcessed()));
	debugger.println("Checksums Failed: " + String(gps_device->failedChecksum()));
	debugger.println("Checksums Passed: " + String(gps_device->passedChecksum()));
	debugger.println("Sentences With Fix: " + String(gps_device->sentencesWithFix()));
	debugger.println("Satellites Found: " + String(gps_device->satellites.value()));
	debugger.println("");
}


String GPS_Data::to_string(void)
{
	// debugger.println("gps>to_string");	
	String locationStr = String(location.lat(), 6) + "," + String(location.lng(), 6);
	String timeStr = String(time.hour()) + "," + String(time.minute()) + "," + String(time.second());
	// debugger.println("to_string_finished");	
	return String(timestamp) + "," + locationStr + "," + timeStr;
}