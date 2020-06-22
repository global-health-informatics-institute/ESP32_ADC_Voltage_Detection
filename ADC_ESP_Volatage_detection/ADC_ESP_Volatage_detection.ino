const int Analog_channel_pin= 15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

// get maximum reading value
int get_max() {
  int Vmax = 0;
  int ADC_VALUE = 0; 
  
  Vmax = 0;
  for(int i = 0; i < 80; i++) {
     ADC_VALUE = analogRead(Analog_channel_pin);  // read from analog channel 3 (A3)
    if(ADC_VALUE > Vmax) Vmax = ADC_VALUE;
    delayMicroseconds(250);
  }  
  return Vmax;
}

void loop() {
  
  Serial.println("ADC Value:" + String(get_max()*407*0.7071/4096));
  delay(1000);

  

}
