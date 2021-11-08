#include <mega16.h>
char stepsArray[] = {0x09, 0x05, 0x06, 0x0a};
unsigned char k = 1;
bit dir = 0;

interrupt[TIM0_OVF] void tim0_ovf_isr()
{
    TCCR0 = 0x00;
    if (dir != 0)
    {
        if (k == 0)
            k = 4;
        else
            k--;
    }
    else
    {
        k++;
        if (k == 4)
            k = 0;
    }
    PORTB = stepsArray[k];
    TCCR0 = 0x05;
    TCNT0 = 0;
}
void main(void)
{
    DDRB = 0xff;
    PORTB = stepsArray[k];
    PORTC = 0xff;
    DDRC = 0x00;
    TIMSK = 0x01;

#asm("sei")
    TCNT0 = 0;
    TCCR0 = 0x05;
    while (true)
    {
        if (PINC .2 == 0)
        {
            while (PINC .2 == 0)
                ;
            dir = ~dir;
        }
    }
}
