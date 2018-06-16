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

Sound::Signal signal{};
Sound::Wave sound{Note::C, 1.};
CantusFirmusGenerator generator{};

volatile bool next_note = false;
#ifndef TEST

ISR(TIMER2_COMPA_vect) {
  HW::audio_out(signal.next());
  signal.position++;
  signal.swap_if_reached_end();
}

ISR(TIMER0_COMPA_vect) {
  static unsigned short count = 0;
  ++count;
  if(count != 500/4)
  {
    return;
  }
  cli();
  count = 0;
  next_note = true;
  sei();
}
#endif

int main()
{
  DEBUG("starting main");
  sound = Sound::Wave{Note::C,1.};
  Sound::LookupTable table{};
  signal = Sound::Signal(sound);
  HW::configure_pins();
  DEBUG("starting loop");

  while (true)
  { 
    if(signal.needs_prepare())
    {
      signal.prepare();
      //HW::toggle_led_if(100);
    }
    if(next_note)
    {
      sound = Sound::Wave{generator.next_note(), 1.};
      signal.sound = sound;
      next_note = false;
    }
    
  }

  return 0;
}
//---------------------------------------------------------------------------
