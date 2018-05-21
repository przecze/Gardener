#pragma once
#include <unique_ptr.h>
#include <defines.h>
class Sound
{
 public:
  //unique_array_ptr<short> data;
  double frequency; // in Hz
  double amplitude; // in V
  double start; // in ms
  double end; // in ms
  int length;
  Sound * child;

  Sound& add( Sound * sound )
  {
    child = sound;
    return *sound;
  }

  Sound(double frequency, double amplitude = 5.):
    frequency(frequency), amplitude(amplitude), child(nullptr)
  {
  }

  double localAmplitude(int x) const
  {
    auto ret = amplitude*cos(x*frequency);
    if (child != nullptr) ret+= child->localAmplitude(x);
    return ret;
  }


  ~Sound()
  {
  }
};

class Signal
{
 public:
  short data[SIGNAL_LENGTH];
  Signal(const Sound & sound)
  {
    double amps[SIGNAL_LENGTH];
    double max_amp = 0;
    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = sound.localAmplitude(i);
      if(max_amp < amp) max_amp = amp;
      amps[i] = amp;
    }
    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = amps[i];
      short norm_amp = short(amp/max_amp * ANALOG_RANGE + 0.5);
      data[i] = norm_amp;
    }
  }
};
