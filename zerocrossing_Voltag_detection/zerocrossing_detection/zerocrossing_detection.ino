#include <MCP3XXX.h>

MCP3002 adc;

int LED_BUILTIN=2;
gpio_num_t zero_cross = GPIO_NUM_35; // THIS IS FOR THE ZERO CROSSING DETECTION AS PER PCB LAYOUT  *** CHANGED TO GPIO25 to match PCB ***
gpio_num_t ELEMENT_firing_pin = GPIO_NUM_33;
bool zero_cross_detected = false;
float Voltage = 1.2;
unsigned long Last_Zero_Crossing_Time = 0;
hw_timer_t *timer = NULL;/* create a hardware timer */
volatile byte state = LOW;/* LED state */

void IRAM_ATTR onTimer(){
  Voltage = adc.analogRead(0) / 1024.0*407*0.7071;
}

void IRAM_ATTR zero_crossing() {
  if ((millis() - Last_Zero_Crossing_Time ) < 2) { // the last interrupt was HIGH to LOW transition since within 2mS
    zero_cross_detected = true;
    timerAlarmEnable(timer);/* Start an alarm */
  }  
    Last_Zero_Crossing_Time = millis();  
}

void setup() 
{
  Serial.begin(115200);
  pinMode (zero_cross, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ELEMENT_firing_pin, OUTPUT);  
    adc.begin(2,23,19,18);// Use the defined pins for SPI hardware interface.
  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true); /* Attach onTimer function to our timer */
  /* Set alarm to call onTimer function every second 1 tick is 1us => 1 second is 1000000us */
    timerAlarmWrite(timer, 5200, false);/* Repeat the alarm (third parameter) */
    attachInterrupt(digitalPinToInterrupt(zero_cross), zero_crossing, RISING);
}

void loop() 
{     
  //If the zero cross interruption was detected we create the 100us firing pulse  
  if (zero_cross_detected){
    zero_cross_detected = false;
    digitalWrite(ELEMENT_firing_pin,HIGH);
    delayMicroseconds(100);
    digitalWrite(ELEMENT_firing_pin,LOW);
  }  
   Serial.println(Voltage);
}
//End of void loop
