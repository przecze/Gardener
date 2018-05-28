// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <unique_ptr.h>
#include <Sound.h>
#include <defines.h>
//#include <Sound.h>

//---------------------------------------------------------------------------
void configure_pins()
{
    DDRB |= (1 << DDB1)|(1 << DDB2);
    // PB1 and PB2 is now an output

    ICR1 = ANALOG_RANGE;
    TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
    // set none-inverting mode

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);
    // set Fast PWM mode using ICR1 as TOP
    
    TCCR1B |= (1 << CS10);
    // START the timer with no prescaler

}


void play(unsigned short * signal)
{
  unsigned short to_set;
  for(int i=0; i < SIGNAL_LENGTH; i+= 1)
  {
    to_set = i;
    to_set  = signal[i];
    OCR1A = to_set;
    OCR1B = to_set;
    _delay_us(TIME_RES_US);
  }

}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  while ( true )
  {
    Sound sound{440., 1.};
    //Sound s2{480., 0.0};
    //sound.add(&s2);
    Signal sig(sound);
    for(int i = 0; i<100; ++i) play(sig.data);
    //Sound s3{440., 1.};
    //Sound s4{660., .2};
    //s3.add(&s4);
    //Signal sig2(s3);
    //for(int i = 0; i<100; ++i) play(sig2.data);
  }

  return 0;
}
//---------------------------------------------------------------------------
