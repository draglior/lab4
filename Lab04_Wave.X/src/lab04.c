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
volatile float phase = 0.0f;
volatile float phase_inc = 0.0f;

#define TCKPS_1   0x00
#define TCKPS_8   0x01
#define TCKPS_64  0x02
#define TCKPS_256 0x03

#define F_SIGNAL 10.0f
#define F_SAMPLE 300.0f
#define V_MIN 1.0f
#define V_MAX 3.0f
#define TWO_PI 6.283185307f

void timer_initialize(void)
{
    T3CONbits.TON = 0;

    T3CONbits.TCS = 0; 
    T3CONbits.TCKPS = TCKPS_1;

    TMR3 = 0;
    PR3 = (12800000 / F_SAMPLE) - 1; //ticks for 300 samples per second

    IFS0bits.T3IF = 0;
    IPC2bits.T3IP = 1;
    IEC0bits.T3IE = 1;

    T3CONbits.TON = 1;
}

void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;

    float amplitude = (V_MAX - V_MIN) / 2.0f; // amplitude +- the offset value
    float offset    = (V_MAX + V_MIN) / 2.0f; //mid point of the wave, since controller cant do <0

    float vout = amplitude * sinf(phase) + offset;

    uint16_t vout_mV = vout * 1000.0;
    uint16_t analogValue = dac_convert_milli_volt(vout_mV);
    uint16_t cmd  = 0b0101000000000000 | analogValue; //add the config bits
    dac_send(cmd);

    phase += phase_inc;
    if (phase >= TWO_PI) phase -= TWO_PI; //periodic after 2pi

    TOGGLELED(LED1_PORT);
}

void main_loop(void)
{
    lcd_printf("Lab04: Wave");
    lcd_locate(0, 1);
    lcd_printf("Group: 5");

    phase_inc = TWO_PI * (F_SIGNAL / F_SAMPLE); //increment step value

    while (TRUE)
    {
        // idle; ISR runs the waveform
    }
}
