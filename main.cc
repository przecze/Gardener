// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <unique_ptr.h>
#include <Sound.h>
#include <defines.h>
#include <Notes.h>

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
  int freq = 1;
  double melody[5];
  {
  }

  while ( true )
  {
    for (short i = 0; i<5; ++i)
    {
      freq=melody[i];
      Sound sound{freq, 1.};
      Signal sig(sound);
      for(int dummy = 0; dummy<500; ++dummy) play(sig.data);
    }
  }
  

  return 0;
}
//---------------------------------------------------------------------------
