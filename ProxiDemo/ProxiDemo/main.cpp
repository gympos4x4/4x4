/*
* ProxiDemo.cpp
*
* Created: 19.11.2017 22:38:48
* Author : DELTA-PC
*/
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

bool ambientRead = false;
volatile uint16_t ambience = 0;
bool flash = false;
int main(void)
{
	
	DIDR0 |= 1 << ADC3D; //Disable digital input at pin ADC3 (PB3) to save power
	
	DDRB |= 1 << PB2;
	DDRB |= 1 << PB4;
	PORTB |= 1 << PB4; //Activate IR LED
	
	//Configure ADC
	ADMUX = 0x03; //Set PB3 (ADC3) as ADC input
	ADCSRA |= 1 << ADEN; //Enable ADC
	ADCSRA |= 1 << ADIE; //Enable ADC Interrupt
	ADCSRA |= 0b100; //Set ADC clock prescaler to 1/16
	
	sei();
	
	ADCSRA |= 1 << ADSC; //Start first conversion
	
	
	/* Replace with your application code */
	while (1)
	{
		/*if(lx){
		PORTB |= 1 << PB3;
		}
		else{
		PORTB &= ~(1 << PB3);
		}
		lx = !lx;
		_delay_ms(150);*/
	}
}
void vdelay(unsigned long ms)
{
	for(unsigned long i = 0; i < ms; i++)
	_delay_ms(1);
}
ISR(ADC_vect)
{
	cli();
	uint16_t adc_result = ADCL;
	adc_result |= (ADCH << 8); //Get ADC result
	
	if(ambientRead){
		PORTB |= 1 << PB4; // Enable IR LED
		ambience = adc_result;
		ambientRead = false;
	}
	else
	{
		PORTB &= ~(1 << PB4); // Disable IR LED
		uint16_t adjusted_result = adc_result - ambience;
		if(adjusted_result > 10 && flash){
			PORTB |= 1 << PB2;
		}
		else{
			PORTB &= ~(1 << PB2);
		}
		ambientRead = true;
		if(adjusted_result < 180){
			vdelay(180-adjusted_result);
			flash = !flash;
		}
		else flash = true;
	}
	ADCSRA |= 1 << ADSC; //Start next conversion
	sei();
}
