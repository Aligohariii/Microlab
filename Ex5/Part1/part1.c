#include <mega16.h>

char steppsArray[] = {0x09, 0x05, 0x06, 0x0a};
unsigned char k = 1;
bit wait;

interrupt[TIM0_OVF] void tim0_ovf_isr()
{
    TCCR0 = 0x00;
    k++;
    if (k == 4)
        k = 0;
    PORTB = steppsArray[k];
}
void main(void)
{
    DDRB = 0xff;
    PORTB = steppsArray[k];
    DDRC = 0x00;
    PORTC = 0xff;

    TIMSK = 0x01;
#asm("sei")
    wait = 0;
    while (true)
    {
        if (PINC .2 == 0)
        {
            TCNT0 = -15;
            TCCR0 = 0x05;
        }
    }
}
