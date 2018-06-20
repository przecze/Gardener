#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

namespace HW
{
  void led_on()
  {
    PORTB |= (1<<PB5);
  }
  void led_off()
  {
    PORTB &= ~(1<<PB5);
  }
  void toggle_led()
  {
    PORTB ^= (1<<PB5);
  }
  void toggle_led_if(int max)
  {
    static int c = 0;
    c++;
    if(c==max)
    {
      toggle_led();
      c = 0;
    }
  }

  void error_mode(int times)
  {
    cli();
    led_off();
    while(true)
    {
      _delay_ms(800);
      for(int i = 0; i<times; ++i)
      {
        led_on();
        _delay_ms(100);
        led_off();
        _delay_ms(200);
      }
    }
  }

  void set_pwm()
  {
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

  void set_change_signal_timer()
  {
    constexpr unsigned short overflow = TIME_RES_US/4 - 1;
    OCR2A = overflow;
    TCCR2A |= (1<<WGM21);
    TCCR2B |= (1<<CS22); // prescaler 64
    TIMSK2 |= (1<<OCIE2A);
    // frequency = 16MHz * 1/(64*overflow) = 1 / (TIME_RES_US)
  }

  void set_change_note_timer()
  {
    // Set the Timer Mode to CTC
    TCCR0A |= (1 << WGM01);

    // Set the value that you want to count to
    OCR0A = 120;
    TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

    TCCR0B |= (1 << CS02) | (1<<CS00); //Set prescaler

  }
  void configure_pins()
  {
    set_pwm();
    set_change_signal_timer();
    set_change_note_timer();
    sei();



  }


  void audio_out(unsigned short to_set)
  {
    OCR1A = to_set;
    OCR1B = to_set;
  }
}
