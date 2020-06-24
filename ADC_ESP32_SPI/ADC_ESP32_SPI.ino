
// MCP3002 - Example code using Arduino UNO.
// March 17,2013
// Wiring
//    Pin 2 to /CS
//    Pin 11 to Din
//    Pin 12 to Dout
//    Pin 13 to CLK
//
// Do a conversion on both channels and write results to serial port.
//
// Include the SPI library:
#include <SPI.h>

// Set pin 10 as the slave select for A/D converter
const int slaveSelect = 2;

void setup()
{
  // Initialize serial port for com to host
  Serial.begin (9600);
  // Set the slave select pin as an output
  pinMode (slaveSelect, OUTPUT);
  // Initialize SPI
  SPI.begin();
  SPI.setBitOrder (MSBFIRST);
}

void loop()
{
  // Going to be doing a conversion on both channels (ch0,ch1)
  byte conversionMSB,conversionLSB;
  word chA;
  digitalWrite (slaveSelect,LOW);
  
  // Communication with the device starts when /CS line brought low.
  // The first '1' bit transmitted is the start bit.
  // The next 3 bits define
  //    Single Ended Input / Differential Input
  //    Ch0 / Ch1 if single ended input or polarity if differential
  //    MSBF / LSBF (Most/Least Significan Bit First)
  // The returned value is ten bits long so if the bit pattern transmitted
  // is placed correctly, the two most significant bits will be sent back
  // with the first transfer. the rest on the second transfer.
  conversionMSB = SPI.transfer (0x60);
  conversionLSB = SPI.transfer (0);
  
  chA = word (conversionMSB,conversionLSB);
  digitalWrite (slaveSelect,HIGH);
  Serial.println(chA,DEC);
  //Serial.print("  ");
}
