// Program zaświeca pojedynczą diodę LED
// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>

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


//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  int count = 0;
  while ( true )
  {
    count++;
    for(int i = 0; i<6; ++i)
    {
      if( 1 == ( (count>>i) & 1 ) )
      {
        long_flash();
      }
      else
      {
        short_flash();
      }
    }
    _delay_ms(500);
  }

  return 0;
}
//---------------------------------------------------------------------------
