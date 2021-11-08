#include <mega16.h>
char steppsArray[] = {0x09, 0x05, 0x06, 0x0a};
unsigned char k = 1;
bit start = 0;
bit speed = 0;
bit dir = 0;
unsigned char temp;

interrupt[TIM0_OVF] void tim0_ovf_isr()
{
    TCCR0 = 0x00;
    if (dir == 0)
    {
        k++;
        if (speed == 0)
            temp += 15;
        else
            temp -= 15;
        if (temp == 255)
            speed = 1;
        if (temp == 0)
        {
            speed = 0;
            dir = ~dir;
        }
        if (k == 4)
            k = 0;
    }
    else
    {

        if (speed == 0)
            temp += 15;
        else
            temp -= 15;
        if (temp == 255)
            speed = 1;
        if (temp == 0)
        {
            speed = 0;
            dir = ~dir;
        }
        if (k == 0)
            k = 4;
        else
            k--;
    }
    PORTB = steppsArray[k];
    TCCR0 = 0x05;
    TCNT0 = temp;
}
void main(void)
{
    DDRB = 0xff;
    PORTB = steppsArray[k];
    PORTC = 0xff;
    DDRC = 0x00;
    TIMSK = 0x01;
    temp = 0;
#asm("sei")

    while (true)
    {
        if (PINC .2 == 0)
        {
            while (PINC .2 == 0)
                ;
            start = ~start;
        }
        if (start == 0)
        {
            temp = 0;
            TCNT0 = 0;
            TCCR0 = 0x00;
        }
        else
        {
            TCNT0 = temp;
            TCCR0 = 0x05;
        }
    }
}
