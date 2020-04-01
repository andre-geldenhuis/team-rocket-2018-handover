/**
 * Implementation of a debugger
 * 
 * Setup so that the methods can be
 *  called at any point but will only
 *  execute if debugging is turned on
 * 
 * This is done to keep code clear throughout the system
 */

#include "debugger.h"

void Debugger::init(void)
{
#if DEBUG
    Serial.begin(SERIAL_BAUD_RATE);

    // Waits until a serial connection has been established
	while(!Serial);
    
	Serial.println("Debugger Started");
#endif
}

void Debugger::print(String msg)
{
#if DEBUG
    Serial.print(msg);
#endif
}

void Debugger::println(String msg)
{
#if DEBUG
    Serial.println(msg);
#endif
}

void Debugger::printf(String msg, ...)
{
#if DEBUG
    // Converts String to char*
    int msgLen = msg.length();
    char strBuf[msgLen];
    msg.toCharArray(strBuf, msgLen+1);

    // Replicates standard printf usage
    char buf[255];
    va_list arg;
    va_start(arg, msg);
    vsnprintf(buf, 255, strBuf, arg);
    va_end(arg);
    
    Serial.print(buf);
#endif
}

void I2C_scan(void)
{
	Wire.begin();
	
	// scan for i2c devices
	byte error, address;
	int nDevices;

	Serial.println("Scanning...");

	nDevices = 0;
	for(address = 1; address < 127; address++ ) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address<16) {
				Serial.print("0");
			}
			Serial.print(address,HEX);
			Serial.println("  !");

			nDevices++;
		}
		else if (error==4) {
			Serial.print("Unknow error at address 0x");
			if (address<16) {
				Serial.print("0");
			}
			Serial.println(address,HEX);
		}
	}
	if (nDevices == 0) {
		Serial.println("No I2C devices found");
	} else {
		Serial.println("done");
	}

}