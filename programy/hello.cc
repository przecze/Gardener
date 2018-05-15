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
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  while ( true )
  {
    _delay_ms(100);
    PORTB |= 1 << PB5; // Włączam diodę.
    _delay_ms(100);
    PORTB &= ~(1 << PB5);
  }

  return 0;
}
//---------------------------------------------------------------------------
