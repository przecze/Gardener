// Autor: Przemysław Czechowski

#include <Sound.h>
#include <defines.h>
#ifdef TEST
  #include <hardware_mock.h>
#else
  #include <hardware.h>
#endif
#include <Notes.h>

class CantusFirmusGenerator
{
  static constexpr char range = 8;
  unsigned char initial_note;
  unsigned char current_note;
  bool allowed[range];
  signed char last_interval =0;
  static constexpr unsigned char NotSet = range+1;
  unsigned char next_note = NotSet;
  bool last_jump_up = false;
  unsigned char allowed_count;
  unsigned char rand = 13;
  void setAll(bool value)
  {
    for(auto i = range-1; i>=0; --i)
    {
      allowed[i] = value;
    }
    allowed_count = range*value;
  }
  void updateAllowed()
  {
    if(last_interval > 3)
    {
      next_note = current_note - 1;
      return;
    }
    if(last_interval < -3)
    {
      next_note = current_note +1;
      return;
    }
    setAll(true);
    for(auto i = range-1; i>=0;--i)
    {
      if(last_jump_up && i > current_note + 1)
      {
        allowed[i] = false;
      }

      if(!last_jump_up && i < current_note - 1)
      {
        allowed[i] = false;
      }

      if(i==current_note)
      {
        allowed[i] = false;
      }
      if(!allowed[i])
      {
        allowed_count--;
      }
    }
    ERROR_CHECK(allowed_count<=1, 7);
    return;
  }

 public:
  CantusFirmusGenerator(): initial_note(Note::C), current_note(initial_note)
  {
    setAll(true);
  }
  double GetNext()
  {
    if(next_note == NotSet)
    {
      rand = (rand*109+89)%range;
      auto position = rand%allowed_count;
      for(auto i = range-1; i>=0;--i)
      {
        if(allowed[i])
        {
          if(position==0)
          {
            next_note = i;
            break;
          }
          else position--;
        }
        ERROR_CHECK(i==0, 6);
      }
    }
    ERROR_CHECK(next_note<0,4);
    ERROR_CHECK(next_note>=range,5);
    last_interval = next_note - current_note;
    if(last_interval > 1)
    {
      last_jump_up = true;
    }
    if(last_interval < -1)
    {
      last_jump_up = false;
    }
    current_note = next_note;
    next_note = NotSet;
    updateAllowed();
    ERROR_CHECK(current_note<0,4);
    ERROR_CHECK(current_note>=range,4);
    return Note::Frequency[current_note];
  }
};

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
