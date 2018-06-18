#pragma once

namespace Note {
  enum Note{
   A  = 0,
   B ,
   C ,
   D ,
   E ,
   F ,
   G ,
   A2,
   B2,
   C2
  };
  double Frequency[] = 
  {
   440,
   493.88,
   523.23,
   587.33,
   659.25,
   698.46,
   783.99,
    880,
    987.7,
    1046.50
  };
  Note& operator++(Note& n,int)
  {
    return n = static_cast<Note>(static_cast<int>(n)+1);
  }
}
