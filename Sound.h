#pragma once
#include <unique_ptr.h>
#include <defines.h>

#ifdef TEST
  #include <cmath>
  #ifndef M_PI
      #define M_PI 3.14159265358979323846
  #endif
#endif

class Sound
{
 public:
  //unique_array_ptr<short> data;
  double frequency; // in Hz
  double amplitude; // in V
  double start; // in ms
  double end; // in ms
  double scale;
  int length;
  Sound * child;

  Sound& add( Sound * sound )
  {
    child = sound;
    return *sound;
  }

  Sound(double frequency, double amplitude = 1.):
    frequency(frequency), amplitude(amplitude), child(nullptr)
  {
    scale = frequency * TIME_RES_US/(1000000.) *2*M_PI;
  }

  double localAmplitude(int x) const
  {
    auto ret = amplitude*cos(scale *x);
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
  unsigned short data[SIGNAL_LENGTH];
  int  phase_i = 0.;
  double max_amp = 0;
  double min_amp = 0;
  Sound sound;
  Signal(Sound sound): sound(sound)
  {
    double amps[SIGNAL_LENGTH];
    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = sound.localAmplitude(i);
      if(max_amp < amp) max_amp = amp;
      if(min_amp > amp) min_amp = amp;
      amps[i] = amp;
    }
    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = amps[i];
      auto tamp = (amp - min_amp)/(max_amp - min_amp);
      short norm_amp = short(tamp * ANALOG_RANGE + 0.5);
      data[i] = norm_amp;
    }
  }
  unsigned short next()
  {
    auto amp = sound.localAmplitude(phase_i);
    auto tamp = (amp - min_amp)/(max_amp - min_amp);
    short norm_amp = short(tamp * ANALOG_RANGE + 0.5);
    phase_i++;
    return norm_amp;
  }
};
