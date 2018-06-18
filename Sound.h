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

namespace Sound
{

constexpr int LOOKUP_SIZE = 500;
constexpr int LOOKUP_MAX = 127;
class LookupTable
{ 
  public:
  LookupTable()
  {
    for(auto _ = LOOKUP_SIZE, i = 0*_; i<LOOKUP_SIZE; ++i)
    {
      double omega = (2*M_PI)/size;
      int8_t a = LOOKUP_MAX*::sin(omega * i) + 0.5;
      data[i] = a;
    }
  }

  double Sin(double phase)
  {

    int index = phase/(2*M_PI)*size;
    int8_t val =  getValue(index);
    double ret = double(val)/LOOKUP_MAX;
    return ret;
  }

  int8_t getValue(unsigned int index)
  {
    return data[index%size];
    index = index % (4*size - 4);
    if(index > 2 * size - 2)
    {
      int8_t ret = - getValue(4*size - 4 - index);
      return ret;
    }
    if(index > size - 1)
    {
      return data[2*size - 2 - index];
    }
    return data[index];
  }

  private:
  int8_t data[LOOKUP_SIZE];
  constexpr static int size = LOOKUP_SIZE;
};

LookupTable table{};//for some reason can't be made static. AVR throws linker errors
double Sin(double x)
{
  return table.Sin(x);
}

class Wave
{
 public:
  double scale_factor;

  Wave(double frequency):
    scale_factor(frequency * TIME_RES_US/(1000000.) *2*M_PI)
  {
  }

  inline double localAmplitude(int x) const
  {
    return Sin(scale_factor *x);
  }
};

class Signal
{
 public:
  int8_t data[SIGNAL_LENGTH];
  int8_t datatwo[SIGNAL_LENGTH];
  unsigned int phase_i = 0;
  volatile int position = 0;
  volatile int8_t * current;
  double max_amp = 1;
  double min_amp = -1;
  Wave sound;
  volatile bool prepared;

  Signal(): sound(0.), prepared(false){}
  Signal(Wave sound): sound(sound), prepared(false)
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
    ERROR_CHECK(!prepared, 3);
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
    ERROR_CHECK(prepared,2);
    phase_i+=SIGNAL_LENGTH;
    int8_t * pnew;
    if (current == data) pnew = datatwo;
    else pnew = data;

    for (int i = 0; i < SIGNAL_LENGTH; ++i)
    {
      auto amp = sound.localAmplitude(phase_i+i);
      auto tamp = (amp - min_amp)/(max_amp - min_amp);
      int8_t norm_amp = int8_t(tamp * ANALOG_RANGE + 0.5);
      pnew[i] = norm_amp;
    }

    prepared = true;
  }

  void swap_if_reached_end()
  {
    if(position==SIGNAL_LENGTH)
    {
      swap_table();
    }
  }
  inline unsigned short next()
  {
      return current[position];
  }
};
}
