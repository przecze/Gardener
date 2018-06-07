// Autor: Przemysław Czechowski

#include <Sound.h>
#include <defines.h>
#ifdef TEST
  #include <hardware_mock.h>
#else
  #include <hardware.h>
#endif
#include <Notes.h>


Signal signal;

#ifdef TEST
template<typename TIMER2_COMPA_vect>
void
#endif
ISR(TIMER2_COMPA_vect) {
  HW::audio_out(signal.next());
  //HW::toggle_led();
}

int main()
{
  HW::configure_pins();
  using namespace Note;
  double melody[] = {C,D,E,F,G,A2,B2,C2};

  int note_i = 0;
  unsigned long long int count = 0;
  Sound sound{Note::C,1.};
  signal = Signal(sound);
  while ( true )
  {
    ++count;
    if(count == 40000)
    {
      DEBUG("changing note");
      count = 0;
      ++note_i;
      if(note_i ==8) note_i = 0;
      sound = Sound{melody[note_i], 1.};
      signal = Signal(sound);
    }
    if(signal.needs_prepare())
    {
      signal.prepare();
    }
  }

  return 0;
}
//---------------------------------------------------------------------------
