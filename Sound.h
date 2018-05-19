#pragma once
#include <unique_ptr.h>
#include <defines.h>
class Sound
{
 public:
  unique_array_ptr<short> data;
  bool prepared;
  double frequency; // in Hz
  double amplitude; // in V
  double start; // in ms
  double end; // in ms
  unique_ptr<Sound> child; // when the destructor is called, also all the children will be destroyed

  Sound& add( Sound* sound )
  {
    if (!child.isNull())
    {
      child->add(sound);
    }
    else
    {
      child = unique_ptr<Sound>{sound};
    }
    return *sound;
  }

  Sound& add(double frequency, double amplitude = 5.)
  {
    return add(new Sound(frequency, amplitude));
  }

  Sound(double frequency, double amplitude = 5.):
    prepared(false), frequency(frequency), amplitude(amplitude), child()
  {
  }

  double localAmplitude(int x)
  {
    auto ret = amplitude*cos(x*frequency);
    if (!child.isNull()) ret+= child->localAmplitude(x);
    return ret;
  }

  bool prepare(int duration)
  {
    auto uamps = unique_array_ptr<double>(new double[duration]);
    auto amps = uamps.get();
    auto norm_amps = new short[duration];
    double max_amp = 0;
    for (int i = 0; i < duration; ++i)
    {
      auto amp = localAmplitude(i);
      if(max_amp < amp) max_amp = amp;
      amps[i] = amp;
    }
    for (int i = 0; i < duration; ++i)
    {
      auto amp = amps[i];
      short norm_amp = short(amp/max_amp * ANALOG_RANGE + 0.5);
      norm_amps[i] = norm_amp;
    }
    data.reset(norm_amps);
    prepared = true;

    return false; //not implemented
  }

  ~Sound()
  {
  }
};
