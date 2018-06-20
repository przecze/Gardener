// Autor: Przemysław Czechowski

#include <Sound.h>
#include <defines.h>
#include <Notes.h>
#include <CantusFirmusGenerator.h>


Sound::Signal signal{};
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
  Sound::Wave sound = Sound::Wave{440.};
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
      sound = Sound::Wave{generator.GetNext()};
      //sound.add(&base);
      signal.sound = sound;
      next_note = false;
    }
    
  }

  return 0;
}
//---------------------------------------------------------------------------
