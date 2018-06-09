// Autor: Przemysław Czechowski

#include <Sound.h>
#include <defines.h>
#ifdef TEST
  #include <hardware_mock.h>
#else
  #include <hardware.h>
#endif
#include <Notes.h>

double melody[] = {Note::C,Note::D,Note::E,Note::F,Note::G,Note::A2,Note::B2,Note::C2};
class CantusFirmusGenerator
{
 public:
  unsigned short note_i = 0;
  CantusFirmusGenerator()
  {
  }
  double next_note()
  {
      ++note_i;
      if(note_i ==8)
      {
        note_i = 0;
      }
      return melody[note_i];
  }
};

Signal signal;
Sound sound{Note::A, 1.};
CantusFirmusGenerator generator{};

#ifdef TEST
template<typename TIMER2_COMPA_vect>
void
#endif

ISR(TIMER2_COMPA_vect) {
  cli();
  HW::audio_out(signal.next());
  signal.position++;
  signal.swap_if_reached_end();
  sei();
}

ISR(TIMER0_COMPA_vect) {
  //cli();
  //static unsigned short count = 0;
  //++count;
  //if(count != 500/16)
  //{
  //  return;
  //}
  //HW::toggle_led();
  //sei();
  //cli();
  //auto freq = generator.next_note();
  //DEBUG("changing note to " << freq);
  //count = 0;
  //sound = Sound{freq, 1.};
  //signal = Signal{sound};
  //sei();
}

int main()
{
  sound = Sound{Note::A,1.};
  signal = Signal(sound);
  HW::configure_pins();
  DEBUG("starting loop");
  while (true)
  { 
    //HW::toggle_led_if(10000);
    if(signal.needs_prepare())
    {
      //HW::toggle_led_if(1);
      signal.prepare();
    }
    
  }

  return 0;
}
//---------------------------------------------------------------------------
