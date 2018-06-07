#pragma once

#define ANALOG_RANGE 32
#define SIGNAL_LENGTH 150
#define TIME_RES_US 100
#ifdef TEST
  #include <iostream>
  #define DEBUG(_msg) do{std::cout<<_msg<<std::endl;}while(false)
#else
  #define DEBUG(_msg)
#endif
