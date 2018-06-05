#pragma once
#include <defines.h>

#ifdef TEST
  #include <cmath>
  #ifndef M_PI
      #define M_PI 3.14159265358979323846
  #endif
#else
  #include <math.h>
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
};

class Signal
{
 public:
  unsigned short data[SIGNAL_LENGTH];
  int phase_i = 0;
  volatile int position = 0;
  double max_amp = 0;
  double min_amp = 0;
  Sound sound;
  volatile bool needsSetting;

  Signal(): sound(0.,0.), needsSetting(true){}
  Signal(Sound sound): sound(sound), needsSetting(true)
  {
    set();
  }

  void set()
  {
    phase_i+=position;
    position = 0;
    double amps[SIGNAL_LENGTH];
    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = sound.localAmplitude(phase_i+i);
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
    needsSetting = false;
  }

  unsigned short next()
  {
    position++;
    if( position < SIGNAL_LENGTH )
    {
      if( position != SIGNAL_LENGTH-10 ) return data[position];
      needsSetting = true;
      return data[position];
    }
    return ANALOG_RANGE;
  }
};
