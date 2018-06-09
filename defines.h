#pragma once

#define ANALOG_RANGE 16
#define SIGNAL_LENGTH 200
#define TIME_RES_US 200
#ifdef TEST
  #include <iostream>
  #define DEBUG(_msg) do{std::cout<<_msg<<std::endl;}while(false)
#else
  #define DEBUG(_msg)
#endif
