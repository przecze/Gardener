#include "defines.h"
#include "Sound.h"
#include "Notes.h"

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
  using namespace Note;
  double melody[] = {A,B,C,D,E,F,G,A2};
  for (short i = 0; i<8; ++i)
  {
    double freq=melody[i];
    Sound sound{freq, 1.};
    Signal sig(sound);
    std::cout<<"asdfasdfasdf: "<<freq<<std::endl;
    play(sig.data);
  }
  return 0;
}
