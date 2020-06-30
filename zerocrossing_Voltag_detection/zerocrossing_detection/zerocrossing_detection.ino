#include <MCP3XXX.h>

MCP3002 adc;

int LED_BUILTIN=2;
gpio_num_t zero_cross = GPIO_NUM_35; // THIS IS FOR THE ZERO CROSSING DETECTION AS PER PCB LAYOUT  *** CHANGED TO GPIO25 to match PCB ***
gpio_num_t ELEMENT_firing_pin = GPIO_NUM_33;

bool zero_cross_detected = false;
float Voltage = 1.2;
float Last_Zero_Crossing_Time = 0.0;

/* create a hardware timer */
hw_timer_t *timer = NULL;

/* LED state */
volatile byte state = LOW;

void IRAM_ATTR onTimer(){
  Voltage = adc.analogRead(0) / 1024.0*407*0.7071;
}

//Zero Crossing Interrupt Function
//void IRAM_ATTR zero_crossing()
//{
// delayMicroseconds(10);
// if (gpio_get_level(zero_cross)) {
//  
//  zero_cross_detected = true;
//  
//  /* Start an alarm */
//  timerAlarmEnable(timer);
//   
// }
//    
//}

void IRAM_ATTR zero_crossing() {
  if ((millis() - Last_Zero_Crossing_Time ) < 2) // the last interrupt was HIGH to LOW transition since within 2mS
    timerAlarmEnable(timer);/* Start an alarm */
  Last_Zero_Crossing_Time = millis();
}

void setup() 
{
  Serial.begin(9600);
  pinMode (zero_cross, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ELEMENT_firing_pin, OUTPUT);
  
  // Use the default SPI hardware interface.
    adc.begin(2,23,19,18);
  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);

  /* Attach onTimer function to our timer */
    timerAttachInterrupt(timer, &onTimer, true);
  
  /* Set alarm to call onTimer function every second 1 tick is 1us
    => 1 second is 1000000us */
    /* Repeat the alarm (third parameter) */
    timerAlarmWrite(timer, 4900, false);

    
    
    attachInterrupt(digitalPinToInterrupt(zero_cross), zero_crossing, RISING);
}


// get maximum reading value
int get_max() {
  int Vmax = 0;
  int ADC_VALUE = 0; 
  
  Vmax = 0;
  for(int i = 0; i < 200; i++) {
     ADC_VALUE = adc.analogRead(0);  // read from adc channel 0
    if(ADC_VALUE > Vmax) Vmax = ADC_VALUE;
  }  
  delayMicroseconds(100);
  return Vmax;
}

void loop() 
{   
  
  //If the zero cross interruption was detected we create the 100us firing pulse  
  if (zero_cross_detected){
    zero_cross_detected = false;
  }
  
   Serial.println("Voltage: " + String(Voltage));    
   Serial.println("Timer: " + String(Last_Zero_Crossing_Time));
   digitalWrite(ELEMENT_firing_pin,HIGH);
   delayMicroseconds(100);
   digitalWrite(ELEMENT_firing_pin,LOW);
   //delay(1000);
}
//End of void loop
