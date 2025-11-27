#include "lab04.h"

#include <xc.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "math.h"

#include "types.h"
#include "lcd.h"
#include "led.h"
#include "dac.h"

// signal parameter

volatile uint32_t interrupt_counter = 0;

/*
 * Timer code
 */

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

void timer_initialize()
{
    __builtin_write_OSCCONL(OSCCONL | 2);
    
    CLEARBIT(T3CONbits.TON); 
    SETBIT(T3CONbits.TCS); 
    T3CONbits.TCKPS = 0b00;
    T3CONbits.TCS=1; //external 32kHz
    TMR3 = 0;
    PR3 = 31; //32 ticks = 1 ms (32768/1000)

    IFS0bits.T3IF = 0;
    IPC2bits.T3IP = 0x01;
    SETBIT(IEC0bits.T3IE);
    SETBIT(T3CONbits.TON);
}

/*
 * main loop
 */

void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
    interrupt_counter++;
}

void main_loop()
{   
    uint16_t i;
    uint16_t t;
    uint32_t cmd;
    
    // print assignment information
    lcd_printf("Lab04: Wave");
    lcd_locate(0, 1);
    lcd_printf("Group: 5");
    
    while(TRUE) {
        lcd_locate(0, 3);
        lcd_printf(" value: %u", interrupt_counter);
        
        for (t = 0; t < 30; t++)
        {

            dac_send(2*sin(2*3.142*t), interrupt_counter);
        }

        }
}
