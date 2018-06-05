#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
namespace HW
{
  void configure_pins()
  {
    // Turn on timer with no prescaler on the clock for fastest
    // triggering of the interrupt service routine.
    TCCR2B |= (1<<CS20);
    TIMSK2 |= _BV(TOIE2);

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
      
      TCCR1B |= (1 << CS10);
      // START the timer with no prescaler

  }


  void audio_out(unsigned short to_set)
  {
    OCR1A = to_set;
    OCR1B = to_set;
    _delay_us(TIME_RES_US);
  }
}
