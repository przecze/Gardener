#pragma once
#include <defines.h>

#ifdef TEST
  #include <hardware_mock.h>
#else
  #include <hardware.h>
#endif

#ifdef TEST
  #include <cmath>
  #ifndef M_PI
      #define M_PI 3.14159265358979323846
void sei(){}
void cli(){}
  #endif
#else
  #include <math.h>
  #include <avr/interrupt.h>
#endif

constexpr int LOOKUP_SIZE = 500;
constexpr int LOOKUP_MAX = 126;
class LookupTable
{ 
  public:
  LookupTable()
  {
    for(auto _ = LOOKUP_SIZE, i = 0*_; i<LOOKUP_SIZE; ++i)
    {
      double omega = (M_PI/2)/LOOKUP_SIZE;
      data[i] =  LOOKUP_MAX*sin(omega * i) + 0.5;
    }
  }

  short sin(double phase)
  {
    int index = phase/(M_PI/2)*LOOKUP_SIZE;
    return getValue(index)/LOOKUP_MAX;
  }

  short getValue(int index)
  {
    DEBUG("index "<<index <<"requested");
    index = index % (4*size - 4);
    DEBUG("after modulo index "<<index <<"requested");
    if(index > 2 * size - 2)
    {
      DEBUG("changing to "<<  4*size - 4 - index);
      return  - getValue(4*size - 4 - index);
    }
    if(index > size - 1)
    {
      DEBUG("changing to "<<  2*size - 2 - index);
      return data[2*size - 2 - index];
    }
    DEBUG("returning "<< index);
    return data[index];
  }

  inline static LookupTable& get()
  {
    static LookupTable table{};
    return table;
  }
  private:
  short data[LOOKUP_SIZE];
  constexpr static int size = LOOKUP_SIZE;
};

class Sound
{
 public:
  //unique_array_ptr<short> data;
  double frequency; // in Hz
  double amplitude; // in V
  double scale_factor;
  Sound * child;
  LookupTable* table;

  Sound& add( Sound * sound )
  {
    child = sound;
    return *sound;
  }

  Sound(double frequency, double amplitude = 1., LookupTable* table = nullptr):
    frequency(frequency), amplitude(amplitude), child(nullptr), table(table)
  {
    scale_factor = frequency * TIME_RES_US/(1000000.) *2*M_PI;
  }

  double localAmplitude(int x) const
  {
    auto ret = amplitude*table->sin(scale_factor *x);
    if (child != nullptr) ret+= child->localAmplitude(x);
    return ret;
  }
};

class Signal
{
 public:
  unsigned short data[SIGNAL_LENGTH];
  unsigned short datatwo[SIGNAL_LENGTH];
  int phase_i = 0;
  volatile int position = 0;
  volatile unsigned short * current;
  double max_amp = 1;
  double min_amp = -1;
  Sound sound;
  volatile bool prepared;

  Signal(): sound(0.,0.), prepared(false){}
  Signal(Sound sound): sound(sound), prepared(false)
  {
    current = datatwo;
    prepare(); //data is prepared
    swap_table(); //current changed to data, prepared to false
  }

  inline bool needs_prepare()
  {
    return !prepared;
  }

  void swap_table()
  {
    position = 0;
    if (current == data)
    {
      current = datatwo;
    }
    else
    {
      current = data;
    }
    prepared = false;
  }

  void prepare()
  {
    phase_i+=SIGNAL_LENGTH;
    unsigned short * pnew;
    if (current == data) pnew = datatwo;
    else pnew = data;

    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = sound.localAmplitude(phase_i+i);
      auto tamp = (amp - min_amp)/(max_amp - min_amp);
      short norm_amp = short(tamp * ANALOG_RANGE + 0.5);
      pnew[i] = norm_amp;
    }

    prepared = true;
  }

  void swap_if_reached_end()
  {
    if(position==SIGNAL_LENGTH)
    {
      HW::toggle_led_if(100);
      swap_table();
    }
  }
  inline unsigned short next()
  {
      return current[position];
  }
};
