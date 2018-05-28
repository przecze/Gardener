#include "defines.h"
#include "Sound.h"

#include <iostream>

void play(unsigned short * data)
{
  for(int i = 0; i < SIGNAL_LENGTH; ++i)
  {
    std::cout<<data[i]<<" ";
  }
  std::cout<<std::endl;
}

int main()
{
  Sound sound{440., 1.};
  Signal sig(sound);
  play(sig.data);
  return 0;
}
