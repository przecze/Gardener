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


void play(Signal& signal)
{
  auto to_set  = signal.next();
  OCR1A = to_set;
  OCR1B = to_set;
  _delay_us(TIME_RES_US);
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  using namespace Note;
  double melody[] = {C,D,E,F,G,A2,B2,C2};


  while ( true )
  {
    for (short i = 0; i<8; ++i)
    {
      double freq=melody[i];
      Sound sound{freq, 1.};
      Signal sig(sound);
      Signal sig2(sound);
      for(int dummy = 0; dummy<100*100; ++dummy) play(sig);
      for(int dummy = 0; dummy<100*100; ++dummy) play(sig2);
    }
  }
  

  return 0;
}
//---------------------------------------------------------------------------
