/*
 * main.c
 *
 *  Created on: Oct 6, 2015
 *      Author: JohnnyP
 *      http://maxembedded.com/2011/06/avr-timers-timer0/ with modifications
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define TCCR0B (*(volatile uint8_t *)((0x33) + 0x20))

// global variable to count the number of overflows
volatile uint8_t tot_overflow;

// initialize timer, interrupt and variable
void timer0_init()
{
    // set up timer with prescaler = 256
	TCCR0B |= (1 << CS02);

    // initialize counter
    TCNT0 = 0;

    // enable overflow interrupt
    TIMSK |= (1 << TOIE0);

    // enable global interrupts
    sei();

    // initialize overflow counter variable
    tot_overflow = 0;
}

// TIMER0 overflow interrupt service routine
// called whenever TCNT0 overflows
ISR(TIMER0_OVF_vect)
{
    // keep a track of number of overflows
    tot_overflow++;
}

int main(void)
{
    // connect led to pin PC0
    DDRB |= (1 << PINB3);

    // initialize timer
     timer0_init();

    // loop forever
    while(1)
    {
        // check if no. of overflows = 12
        if (tot_overflow >= 12)  // NOTE: '>=' is used
        {
            // check if the timer count reaches 53
            if (TCNT0 >= 53)
            {
                PORTB ^= (1 << PINB3);    // toggles the led
                TCNT0 = 0;            // reset counter
                tot_overflow = 0;     // reset overflow counter
            }
        }
    }
}

