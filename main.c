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

// Prescaler on page 80
/* 	CS02 	CS01 	CS00 	Description
	0 		0 		0 		No clock source (Timer/Counter stopped)
	0 		0 		1 		clk I/O /(No prescaling)
	0		1 		0 		clk I/O /8 (From prescaler)
	0 		1 		1 		clk I/O /64 (From prescaler)
	1 		0 		0 		clk I/O /256 (From prescaler)
	1 		0 		1 		clk I/O /1024 (From prescaler)
	1 		1 		0 		External clock source on T0 pin. Clock on falling edge.
	1 		1		1 		External clock source on T0 pin. Clock on rising edge.*/

// To obtain 0.5 second period on ATtiny85 with default internal 1 MHz clock
// No prescaler was used with total overflows of 1953 and 32 additional ticks
// For calculations use: http://eleccelerator.com/avr-timer-calculator/
// Using prescaler decreases counter resolution.

// global variable to count the number of overflows
volatile uint16_t overflowCount;

// initialize timer, interrupt and variable
void timer0Initialization()
{
    // set up timer with prescaler = 256
	TCCR0B |= (1 << CS00);

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
        if (overflowCount >= 1953)
        {
            if (TCNT0 >= 32)
            {
                PORTB ^= (1 << PINB3);    	// toggles the LED on PC0
                TCNT0 = 0;            		// reset timer
                overflowCount = 0;     		// reset overflow counter
            }
        }
    }
}

