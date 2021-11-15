#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lcd.h"

enum Mode  

{
    TEMP,      
    SET_MIN_H, 
    SET_MAX_H 
};

int BV(int bit);
void change_value();

void show_lcd();

char mode = TEMP; 

int ref = 1; 

int max_h = 25; 

int min_h = 20;

int value;

int main() 

{
    DDRB = 0x01; 

    LCD_Init();
 
    GICR = BV(INT2);  
    
    MCUCSR = (1 << ISC2); 

    sei();                

    ADCSRA = BV(ADEN) | BV(ADPS2) | BV(ADPS1) | BV(ADPS0); 

    ADMUX = BV(REFS0); 

    while (1)

    {
        ADCSRA |= BV(ADSC); 

        while ((ADCSRA & BV(ADIF)) == 0)
            ; 


        value = ADCW * ref / 10; 

        change_value();

        _delay_ms(10);
    }

    return 0;
}


int BV(int bit)
{
    return 1 << bit;
}

// interrupt 2 function
ISR(INT2_vect)
{
    if ((PINB & BV(7)) != 0) 

    {
        if (mode == TEMP)
            mode = SET_MIN_H;
        else if (mode == SET_MIN_H)
            mode = SET_MAX_H;
        else
            mode = TEMP;

        LCD_Clear(); 

    }
    if ((PINB & BV(5)) != 0) 
    {
        if (mode == SET_MIN_H)
        {
            min_h++;
            if (min_h > max_h)

                max_h = min_h;
        }
        if (mode == SET_MAX_H)
            max_h++;
    }
    if ((PINB & BV(6)) != 0) 
    {
        if (mode == SET_MIN_H)
            min_h--;
        if (mode == SET_MAX_H)
        {
            if (max_h > min_h) 

                max_h--;
        }
    }

    show_lcd();
}

void change_value()
{
    if (value >= max_h)
        PORTB = 0x01; 


    if (value <= min_h)
        PORTB = 0x00; 

    if (mode == TEMP)
        show_lcd();
}

void show_lcd()

{
    char str[15];

    if (mode == TEMP)
    {
        sprintf(str, "Temp: %5d C", value);

        if ((PORTB & 0x01) != 0)
            str[13] = '!'; 
        else
            str[13] = ' ';
    }
    else if (mode == SET_MIN_H)
    {
        sprintf(str, "Min H: %5d C ", min_h); 
    }
    else if (mode == SET_MAX_H)
    {
        sprintf(str, "Max H: %5d C ", max_h); 
    }

    LCD_Command(0x02); 

    LCD_String(str);   
}