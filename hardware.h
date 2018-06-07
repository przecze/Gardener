#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
namespace HW
{
  void toggle_led()
  {
    static int c = 0;
    cli();
    c++;
    if(c==10000)
    {
      PORTB ^= (1<<PB5);
      c = 0;
    }
    sei();
  }

  void configure_pins()
  {

    OCR2A = 200 - 1;
    TCCR2A |= (1<<WGM21);
    TCCR2B |= (1<<CS21); // prescaler 8
    TIMSK2 |= (1<<OCIE2A);
    // frequency = 16MHz / (8*200) = 0.01 MHz = 1 / (100us)

    // Turn interrupts on.
    sei();

      DDRB |= (1 << DDB1)|(1 << DDB2);
      // PB1 and PB2 is now an output

      ICR1 = ANALOG_RANGE;
      TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
      // set none-inverting mode

      TCCR1A |= (1 << WGM11);
      TCCR1B |= (1 << WGM12)|(1 << WGM13);
      // set Fast PWM mode using ICR1 as TOP
      
      TCCR1B |= (1 << CS00);
      // START the timer with no prescaler

  }


  void audio_out(unsigned short to_set)
  {
    cli();
    OCR1A = to_set;
    OCR1B = to_set;
    sei();
  }
}
