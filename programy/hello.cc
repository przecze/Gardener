// Program zaświeca pojedynczą diodę LED
// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>

class Sound
{
  int * data;
  int length;
  Sound(int f)
  {
    data = new int[length];
    for( int i = 0; i < length; ++i)
    {

    }
  }
  ~Sound()
  {
    for(int i  = 0; i< length; ++i)
    {
      delete &(data[i]);
    }
  }
};

//---------------------------------------------------------------------------
void configure_pins()
{
  // Ustaw wyprowadzenie PA0 w tryb wyjścia.
  // Wyprowadzenie PA1 jest początkowo ustawione jako wejściowe
  // bez rezystora podciągającego, zatem dodatkowa konfiguracja nie jest
  // potrzebna.
}
void short_flash()
{
    _delay_ms(140);
    PORTB |= 1 << PB5; // Włączam diodę.
    _delay_ms(20);
    PORTB &= ~(1 << PB5);
    _delay_ms(140);
}
void long_flash()
{
    _delay_ms(50);
    PORTB |= 1 << PB5; // Włączam diodę.
    _delay_ms(200);
    PORTB &= ~(1 << PB5);
    _delay_ms(50);
}

template<int delay>
void play_sound(long long int count = 500000)
{
  for(long long int i = 0; i<count; i+=2*delay)
  {
    PORTD |= 1 << PD5; // Włączam diodę.
    PORTB |= 1 << PD2; // Włączam diodę.
    PORTB |= 1 << PB5; // Włączam diodę.
    _delay_us(delay);
    PORTD &= ~(1 << PD5);
    PORTB &= ~(1 << PB2);
    PORTB &= ~(1 << PB5);
    _delay_us(delay);
  }
}


//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  while ( true )
  {
    play_sound<1000>();
    play_sound<1000>();
    play_sound<2000>();
  }

  return 0;
}
//---------------------------------------------------------------------------
