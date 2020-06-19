const int Analog_channel_pin= 15;
int ADC_VALUE = 0;
int Vmax = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

// get maximum reading value
int get_max() {
  for(int i = 0; i < 40; i++) {
     ADC_VALUE = analogRead(Analog_channel_pin);  // read from analog channel 3 (A3)
    if(ADC_VALUE > Vmax) Vmax = ADC_VALUE;
  }  
  return Vmax;
}

void loop() {
  
  Serial.print("ADC Value:" + String(get_max()));
  delay(1000);

  

}
