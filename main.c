/*
 * main.c
 *
 *  Created on: Oct 6, 2015
 *      Author: JohnnyP
 *      http://maxembedded.com/2011/07/avr-timers-ctc-mode/ with modifications
 *      http://forum.arduino.cc/index.php?topic=211639.0
 *      http://eleccelerator.com/avr-timer-calculator/
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define TCCR0B (*(volatile uint8_t *)((0x33) + 0x20))
#define TCCR0A (*(volatile uint8_t *)((0x2A) + 0x20))
#define OCR0A (*(volatile uint8_t *)((0x29) + 0x20))
#define WGM01 1
#define OCIE0A 4
#define OCF0A 4

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

// Used Clear Timer on Compare Match (CTC) mode #2 page 79
// To obtain 0.25 second period on ATtiny85 with default internal 1 MHz clock
// 1024 prescaler was used with total counts of 244
// Using prescaler decreases counter resolution. Because the compare value is
// only 8 bit the maximal delay equals to 0.26 ms (without additional routine in TIMER0_COMPA_vect)

// initialize timer, interrupt and variable
void timer0Initialization()
{
    // set up timer with prescaler = 1024 and CTC mode
    TCCR0B |= (1 << CS02)|(1 << CS00);
    TCCR0A |= (1 << WGM01);

    // initialize counter
    TCNT0 = 0;

    // initialize compare value. This gives period of 0.25 ms
    OCR0A = 244;

    // enable compare interrupt
    TIMSK |= (1 << OCIE0A);

    // enable global interrupts
    sei();
}

// this ISR is fired whenever a match occurs
// hence, toggle led here itself..
ISR (TIMER0_COMPA_vect)
{
    // toggle led here
    PORTB ^= (1 << PINB3);
}

int main(void)
{
    // connect led to pin PB3 (Pin #2)
    DDRB |= (1 << PINB3);

    // initialize timer
    timer0Initialization();

    while(1)
    {
        // do nothing
        // whenever a match occurs, ISR is fired
        // toggle the led in the ISR itself
        // no need to keep track of any flag bits here
        // done!
    }
    return 0;
}

