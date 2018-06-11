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
Sound sound{Note::C, 1.};
CantusFirmusGenerator generator{};

#ifdef TEST
template<typename TIMER2_COMPA_vect>
void
#endif

ISR(TIMER2_COMPA_vect) {
  HW::audio_out(signal.next());
  signal.position++;
  signal.swap_if_reached_end();
}

volatile bool next_note = false;
ISR(TIMER0_COMPA_vect) {
  //static unsigned short count = 0;
  //++count;
  //if(count != 500/4)
  //{
  //  return;
  //}
  //cli();
  //count = 0;
  ////auto freq = generator.next_note();
  //next_note = true;
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
    if(signal.needs_prepare())
    {
      signal.prepare();
    }
    //if(next_note)
    //{
    //  sound = Sound{generator.next_note(), 1.};
    //  signal.sound = sound;
    //  next_note = false;
    //}
    
  }

  return 0;
}
//---------------------------------------------------------------------------
