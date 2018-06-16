#include <iostream>

int main()
{
  for(int i = 0; i<100; ++i)
  {
    if(i*i%8 == 0)
    {
      std::cout<<i<<" "<<std::endl;
    }
    else
    {
      std::cout<<"helloworld!"<<std::endl;
    }
  }
  return 0;
}
