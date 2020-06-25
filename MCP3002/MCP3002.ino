//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include <MCP3XXX.h>


MCP3002 adc;


void setup()
{
  Serial.begin(9600);

  // Use the default SPI hardware interface.
  adc.begin();

  // Or use custom pins to use a software SPI interface.
  // adc.begin(SS, MOSI, MISO, SCK);
}

// get maximum reading value
int get_max() {
  int Vmax = 0;
  int ADC_VALUE = 0; 
  
  Vmax = 0;
  for(int i = 0; i < 200; i++) {
     ADC_VALUE = adc.analogRead(0);  // read from adc channel 0
    if(ADC_VALUE > Vmax) Vmax = ADC_VALUE;
    delayMicroseconds(100);
  }  
  return Vmax;
}

void loop()
{

  Serial.println(get_max()*( 4.1 / 1023.0 ));
  delay(1000);
  /*Serial.println(adc.analogRead(0));
  float volts = adc.analogRead(0)* ( 4.1 / 1023.0 );
  Serial.println(volts);
  delay(1000);*/
}
