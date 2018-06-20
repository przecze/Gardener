#pragma once

#define ANALOG_RANGE 64
#define SIGNAL_LENGTH 200
#define TIME_RES_US 200
#ifdef TEST
  #include <iostream>
  #define DEBUG(_msg) do{std::cout<<_msg<<std::endl;}while(false)
#else
  #define DEBUG(_msg)
#endif

#ifdef ERROR_MODE
  #include <hardware.h>
  #define ERROR_CHECK(_expr, _err_no) do{if(_expr) HW::error_mode(_err_no);}while(false)
#else
  #define ERROR_CHECK(_expr, _err_no)
#endif
