#include <MCP3XXX.h>

MCP3002 adc;

int LED_BUILTIN=2;
gpio_num_t zero_cross = GPIO_NUM_35; // THIS IS FOR THE ZERO CROSSING DETECTION AS PER PCB LAYOUT  *** CHANGED TO GPIO25 to match PCB ***
bool zero_cross_detected = false;
float Voltage = 0.0;

/* create a hardware timer */
hw_timer_t *timer = NULL;

/* LED state */
volatile byte state = LOW;

void IRAM_ATTR onTimer(){
  Voltage = adc.analogRead(0) / 1024.0*407*0.7071;
}

//Zero Crossing Interrupt Function
void IRAM_ATTR zero_crossing()
{
 delayMicroseconds(10);
 if (gpio_get_level(zero_cross))
    zero_cross_detected = true;

    /* Start an alarm */
    timerAlarmEnable(timer);
     /* Attach onTimer function to our timer */
    timerAttachInterrupt(timer, &onTimer, true);
}

void setup() 
{
  Serial.begin(115200);
  pinMode (zero_cross, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);
  
  /* Set alarm to call onTimer function every second 1 tick is 1us
    => 1 second is 1000000us */
    /* Repeat the alarm (third parameter) */
    timerAlarmWrite(timer, 5000, true);
    
    attachInterrupt(digitalPinToInterrupt(zero_cross), zero_crossing, RISING);
    
    // Use the default SPI hardware interface.
    adc.begin(2,23,19,18);
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
  return Vmax;
}

void loop() 
{   
  
  //If the zero cross interruption was detected we create the 100us firing pulse  
  if (zero_cross_detected){
    zero_cross_detected = false;
   Serial.println(String(Voltage));
   }
   delay(1000);
}
//End of void loop
