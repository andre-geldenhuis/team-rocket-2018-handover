#include "rf_driver.h"

SPISettings rf_spi_settings(1000000, MSBFIRST, SPI_MODE0);

RF_Module::RF_Module(RH_RF95* libObj)
{
	rf95 = libObj;
}

bool RF_Module::init(void)
{
	SPI.setSCK(SPI_SCK);
	debugger.println("LoRa Init Started");
	
    // Sets the reset pins to output
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	// Manually resets the radio pin
	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	// Initialising and checking the module
	if (rf95->init()) {
		debugger.println("LoRa Init Success");
	} else {
		debugger.println(">>LoRa Init Failed");
		return false;
	}

	// Setting and checking the frequency is correct
	if (!rf95->setFrequency(RF95_FREQ)) {
		debugger.println("> LoRa Frequency Set Failed");
		return false;
	}

	rf95->setTxPower(23, false);

	return true;
}

String RF_Module::write(String msg)
{
	writes++;
	SPI.beginTransaction(rf_spi_settings);
	SPI.transfer('0');
	SPI.endTransaction();

	int msgLen = msg.length();
	char msgChar[msgLen];
	msg.toCharArray(msgChar, msgLen + 1); // converts the message into the appropriate char array for transmission
	
	// debugger.println("Sending: " + msg);

	rf95->send((uint8_t *)msgChar, msgLen+1);
	delay(10);

	// debugger.println("Waiting for packet to complete...");

	// Waits upto three seconds for the packet to be sent
	if(rf95->waitPacketSent(3000)) {
		// debugger.println("Sent");
	} else {
		// debugger.println("Sending failed");
	}

	//Reads but discrads result
	// Serial.println(">> "+read());
	return read();
}

String RF_Module::read(void)
{
	SPI.beginTransaction(rf_spi_settings);
	SPI.transfer('0');
	SPI.endTransaction();
	// debugger.println("LoRa read started");

	//Checks if the radio is available for us to receive
	if (rf95->waitAvailableTimeout(1000)) {

		// Should be a message for us now
		uint8_t len = RH_RF95_MAX_MESSAGE_LEN;
		char buf[len];

		// Formats received message
		if (rf95->recv((uint8_t*)buf, &len)) {
			//str = "Got: " + String(buf) + "\nRSSI: " + rf95->lastRssi();
			// Serial.println(buf);
			return String(buf);
		}
	}
	// debugger.println("LoRa read finished");
	return String("");
}


