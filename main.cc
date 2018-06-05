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
template<typename TIMER2_OVF_vect>
void
#endif
ISR(TIMER2_OVF_vect) {
  HW::audio_out(signal.next());
}

int main()
{
  HW::configure_pins();
  using namespace Note;
  double melody[] = {C,D,E,F,G,A2,B2,C2};

  int note_i = 0;
  int count = 0;
  Sound sound{Note::C,1.};
  signal = Signal(sound);
  while ( true )
  {
    ++count;
    if(count >= 1000)
    {
      DEBUG("changing note");
      count = 0;
      ++note_i;
      if(note_i ==8) note_i = 0;
      sound = Sound{melody[note_i], 1.};
      signal = Signal(sound);
    }
    if(signal.needsSetting)
    {
      DEBUG("Setting signal");
      signal.set();
    }
    //HW::audio_out(signal.next());
    //HW::audio_out(count%(ANALOG_RANGE+1));
  }

  return 0;
}
//---------------------------------------------------------------------------
