#include <c8051F350.h>
#include <include.h>
#include "define.h"
#include "type.h"
#include "modbus.h"
#include "i2c.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
void end_timeout1s();
void start_timeout1s(unsigned int i);
void restartI2C();
/******************************************************************************/
xdata unsigned int xtimer_arr[2];
code struct virtual_timer timer_arr[]={
{&xtimer_arr[0],start_timeout1s,end_timeout1s},
{&xtimer_arr[1],0,restartI2C},//таймер перезапуску І2С при завмиранні
};
/******************************************************************************/
void timer2_ISR (void) interrupt 5 using 2
{
    unsigned char i;
    TF2H = 0;

    for(i=0;i<sizeof(timer_arr)/sizeof(struct virtual_timer);i++)
    {
        if(*timer_arr[i].tick != 0)
        {
            *timer_arr[i].tick -= 1;
            if(*timer_arr[i].tick == 0 && timer_arr[i].end_funct)
                timer_arr[i].end_funct();
        }
    }
}

/******************************************************************************/
#define delta 6
/******************************************************************************/
/******************************************************************************/
//таймер 1 секунда
void start_timeout1s(unsigned int i)
{
    if(!i)*timer_arr[1].tick = 100;  //1.00c
    else *timer_arr[1].tick = i;
}

void end_timeout1s()
{
    //start_timeout1s(0);
}
/******************************************************************************/

/******************************************************************************/
void restartI2C()
{
    i2c_op = 0;
}

/******************************************************************************/