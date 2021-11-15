#include <alcd.h>
#include <mega32.h>
#include <stdio.h>
#include <delay.h>

int temp = 0;

char St[16];

void main(void)
{
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);
    SFIOR = (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
    ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    lcd_init(16);


    while (true)
    
{
        ADMUX = 0 | (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);

        delay_us(10);
      
         ADCSRA |= (1 << ADSC);

        while ((ADCSRA & (1 << ADIF)) == 0)
            ;

        ADCSRA |= (1 << ADIF);

        sprintf(St, "the temperature is=%d  C", ADCH);
        lcd_clear();
        lcd_gotoxy(0, 0);
        lcd_puts(St);
        delay_ms(120);
    }
}
