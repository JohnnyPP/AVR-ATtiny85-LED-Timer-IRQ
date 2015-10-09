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

// To obtain 0.5 second period on ATtiny85 with default internal 1 Mhz clock
// The 256 prescaler was used with total overflows of 7 and 161 additional ticks
// For calculations use: http://eleccelerator.com/avr-timer-calculator/

// global variable to count the number of overflows
volatile uint8_t overflowCount;

// initialize timer, interrupt and variable
void timer0Initialization()
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
    overflowCount = 0;
}

// TIMER0 overflow interrupt service routine
// called whenever TCNT0 overflows
ISR(TIMER0_OVF_vect)
{
    // keep a track of number of overflows
	overflowCount++;
}

int main(void)
{
    // connect led to pin PC0
    DDRB |= (1 << PINB3);

    // initialize timer
    timer0Initialization();

    // loop forever
    while(1)
    {
        if (overflowCount >= 7)
        {
            if (TCNT0 >= 161)
            {
                PORTB ^= (1 << PINB3);    	// toggles the LED on PC0
                TCNT0 = 0;            		// reset timer
                overflowCount = 0;     		// reset overflow counter
            }
        }
    }
}

