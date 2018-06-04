// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <unique_ptr.h>
#include <Sound.h>
#include <defines.h>
#include <Notes.h>


Signal signal;

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


void audio_out(unsigned short to_set)
{
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

  int note_i = 0;
  int count = 0;
  Sound sound{C,1.};
  signal = Signal(sound);
  int h = 1;
  //for(long int i = 0; i<16*10000; ++i)
  //{
  //  if(i%10000 == 0) ++h;
  //  audio_out(i%(h));
  //}
  while ( true )
  {
    ++count;
    if(count >= 1000)
    {
      count = 0;
      ++note_i;
      if(note_i ==8) note_i = 0;
      sound = Sound{melody[note_i], 1.};
      signal = Signal(sound);
    }
    if(signal.needsSetting) signal.set();
    audio_out(signal.next());
  }

  return 0;
}
//---------------------------------------------------------------------------
