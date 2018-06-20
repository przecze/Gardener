#pragma once
#include <defines.h>
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
  unsigned long long int rand = 17;
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
    ERROR_CHECK(allowed_count==0, 7);
    return;
  }
  void GetRandomNext()
  {
      rand = (rand*69069+1)%(1ull<<32);
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

 public:
  CantusFirmusGenerator(): initial_note(Note::C), current_note(initial_note)
  {
    setAll(true);
  }
  double GetNext()
  {
    if(next_note == NotSet)
    {
      for(int i = 0; i<2; ++i)
      {
        GetRandomNext();
        if(next_note - current_note == 1) break;
        else if(next_note - current_note == -1) break;
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
