#include "sdcard_driver.h"

bool SD_Module::init(void)
{
	debugger.println("SD Card Init Started");

	if (SD.begin(BUILTIN_SDCARD)) {
		debugger.println("SD Card Init Success");
		return true;
	} else {
		debugger.println("> SD Card Init Failed");
		return false;
	}
}

bool SD_Module::write(const char* filename, String str)
{
	writes++;
	File dataFile = SD.open(filename, FILE_WRITE);

	if (dataFile) {
		// debugger.println(str);
		
		dataFile.println(str);
		dataFile.close();

		// debugger.println("Finished writing to storage");
	} else {
		debugger.println(">> Error writing \"" + str + "\" to storage");
		return false;
	}
	return true;
}

String SD_Module::read(const char* filename)
{
	File dataFile = SD.open(filename, FILE_READ);
	String data = "";

	if (dataFile) {
		debugger.println("Reading from file");

		size_t i = 0;
		while(dataFile.available() && i < dataFile.size()) {
			data = data.concat((char)dataFile.read());
		}
		dataFile.close();

		debugger.println("Finished reading from file");
	} else {
		
		debugger.println("Error reading from file");
	}

	return data;
}