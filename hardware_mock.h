#pragma once
#include<iostream>
#define ERROR_CHECK(_expr, _err_no) if(_expr){std::cout<<"ERROR no "<<_err_no<<std::endl;}

namespace HW
{
  void configure_pins()
  {
    std::cout<<"configure_pins called"<<std::endl;
  }
  void audio_out(unsigned short amp)
  {
    static bool first = true;
    if( first )
    {
      first = false;
      std::cout<<"analog output: ";
    }
    std::cout<<amp<<" ";
  }
  void toggle_led()
  {
    DEBUG("toggle led");
  }
  void toggle_led_if(int max)
  {
    DEBUG("toggle led if");
  }
}
