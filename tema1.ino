 #include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//    LiquidCrystal lcd(RS, enable, d4, d5, d6, d7); //
//
// The parameters to this initialisation function are the pins on 
// the Arduino board that are connected to the pins on the LCD Module, 
// i.e., the pins on the right column in the table above.
//
// The first parameter is the “RS” — which, literally, means 
//      “Register Select”, and which is used to tell the
//       module if “that which comes over the bus (the set of data pins)
//       corresponds to “data to print” or “control instructions”
//
// The second parameter is “Enable Signal"
//       the remaining parameters are the 4 digital pins forming the bus.
//
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


int ora0=0,ora1=14,min0=5,min1=9,sec0=5,sec1=0;

void adc_init()
{
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
  ADMUX |= (1<<REFS0);
  ADCSRA |= (1<<ADEN);
  ADCSRA |= (1<<ADSC);
}

uint16_t read_adc(uint8_t channel)
{
  
  ADMUX &= 0xf0;
  ADMUX |= channel;
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC));
  return ADC;
}

int main()
{
  adc_init();
 
  OCR1A= 15625;
  lcd.begin(16,2);
  
  TCCR1B|=1<<CS12|1<<CS10|1<<WGM12;
  TIMSK1|=1<<OCIE1A;
  sei();
  while(1)
  {
    
  }
}

ISR(TIMER1_COMPA_vect)
{
  sec1++;
  lcd.setCursor(0, 0);
  lcd.write("Temp = ");
  lcd.setCursor(7, 0);
  lcd.print("         ");
  lcd.setCursor(7, 0);
  lcd.print((read_adc(1)* 0.004882814-0.5)*100,0);
  lcd.write("'");
  lcd.write("C");
  if(sec1>9)
  {
    sec1=0;
    sec0++;
    if(sec0>5)
    {
      sec0=0;
      min1++;
    }
  }
  if(min1>9)
  {
    min1=0;
    min0++;
    if(min0>5)
    {
      min0=0;
      ora1++;
    }
  }
  if(ora1==25)
    ora1=0;
  
  lcd.setCursor(0, 1);
  lcd.print("Ora ");
  lcd.setCursor(4,1);
  if(ora1<10)
  {  
    lcd.print(ora0);
  }  
  lcd.print(ora1);
  lcd.print(":");
  lcd.print(min0);
  lcd.print(min1);
  lcd.print(":");
  lcd.print(sec0);
  lcd.print(sec1);
  
}
