#include <MCP3XXX.h>

MCP3002 adc;

gpio_num_t zero_cross = GPIO_NUM_35; // THIS IS FOR THE ZERO CROSSING DETECTION AS PER PCB LAYOUT  *** CHANGED TO GPIO25 to match PCB ***
gpio_num_t firing_pin = GPIO_NUM_33;
bool zero_cross_detected = false;
bool Voltage_read = false;
float Voltage = 0;
unsigned long previousMicros = 0; 
unsigned long currentMicros = 0;
int voltage_read_Delay = 4800;
float volts = 0;
unsigned long Last_Zero_Crossing_Time = 0;

//Zero Crossing Interrupt Function
void IRAM_ATTR zero_crossing()
{
 delayMicroseconds(10);
 if (gpio_get_level(zero_cross))
    zero_cross_detected = true;
 Last_Zero_Crossing_Time = micros(); 
}

//void IRAM_ATTR zero_crossing() {
//  if ((micros() - Last_Zero_Crossing_Time ) > 2000) { // the last interrupt was HIGH to LOW transition since within 2mS
//    zero_cross_detected = true;
//    //timerAlarmEnable(timer);/* Start an alarm */
//  }  
//    Last_Zero_Crossing_Time = micros();
//}

void setup() 
{
  Serial.begin(115200);  
  adc.begin(2,23,19,18);// Use the defined pins for SPI hardware interface.
  pinMode(firing_pin, OUTPUT); 
  pinMode (zero_cross, INPUT); 
  attachInterrupt(digitalPinToInterrupt(zero_cross), zero_crossing, RISING);
}
void loop() 
{    
  currentMicros = micros(); 
  //If the zero cross interruption was detected we create the 100us firing pulse  
  if (zero_cross_detected){
    Voltage_read = false;
    zero_cross_detected = false; 
    digitalWrite(firing_pin,HIGH);
    delayMicroseconds(100);
    digitalWrite(firing_pin,LOW);
  }

  if (((currentMicros - Last_Zero_Crossing_Time) >= voltage_read_Delay) && (!Voltage_read)) {
    Voltage_read = true;
    volts = adc.analogRead(0) / 1024.0*407*0.7071;    
    if (volts > 20) {
      Serial.println(volts);
    }
  }
}
//End of void loop
