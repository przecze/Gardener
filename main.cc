// Autor: Przemysław Czechowski

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <unique_ptr.h>
#include <defines.h>
#include <Sound.h>

//---------------------------------------------------------------------------
void configure_pins()
{
    DDRB |= (1 << DDB1)|(1 << DDB2);
    // PB1 and PB2 is now an output

    ICR1 = (1 << 3); //sets top 16, update every 1 us

    OCR1A = 0x3FFF;
    // set PWM for 25% duty cycle @ 16bit

    OCR1B = 0xBFFF;
    // set PWM for 75% duty cycle @ 16bit

    TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
    // set none-inverting mode

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);
    // set Fast PWM mode using ICR1 as TOP
    
    TCCR1B |= (1 << CS10);
    // START the timer with no prescaler

}


void play(short * signal)
{
  for(int i=0; i < 32; i+= 1)
  {
    OCR1A = signal[i];
    OCR1B = signal[i];
    _delay_us(100);
  }

}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  Sound sound{440., 1.};
  while ( true )
  {
    play(sound.data.get());
  }

  return 0;
}
//---------------------------------------------------------------------------
