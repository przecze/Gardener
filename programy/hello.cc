// Program zaświeca pojedynczą diodę LED w czasie naciśnięcia przycisku.
//
// Wymagane połączenia w układzie ZL15AVR:
//        PA0 - LED 0
//        PA1 - SW0
//
// Autor: Paweł Klimczewski, 15 stycznia 2010.

#include <avr/io.h>

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
      PORTB |= 1 << PB5; // Włączam diodę.
  }
  return 0;
}
//---------------------------------------------------------------------------
