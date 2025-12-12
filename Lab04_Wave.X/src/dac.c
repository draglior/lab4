#include "dac.h"

// tristate register
#define DAC_CS_TRIS TRISDbits.TRISD8
#define DAC_SDI_TRIS TRISBbits.TRISB10
#define DAC_SCK_TRIS TRISBbits.TRISB11
#define DAC_LDAC_TRIS TRISBbits.TRISB13

// port register
#define DAC_CS_PORT PORTDbits.RD8
#define DAC_SDI_PORT PORTBbits.RB10
#define DAC_SCK_PORT PORTBbits.RB11
#define DAC_LDAC_PORT PORTBbits.RB13

// analog to digital converter 1 port configuration register
#define DAC_SDI_AD1CFG AD1PCFGLbits.PCFG10
#define DAC_SCK_AD1CFG AD1PCFGLbits.PCFG11
#define DAC_LDAC_AD1CFG AD1PCFGLbits.PCFG13

// analog to digital converter 2 port configuration register
#define DAC_SDI_AD2CFG AD2PCFGLbits.PCFG10
#define DAC_SCK_AD2CFG AD2PCFGLbits.PCFG11
#define DAC_LDAC_AD2CFG AD2PCFGLbits.PCFG13

void dac_initialize()
{
    // set AN10, AN11 AN13 to digital mode
    DAC_SDI_AD1CFG = 1;
    DAC_SCK_AD1CFG = 1;
    DAC_LDAC_AD1CFG = 1;

    DAC_SDI_AD2CFG = 1;
    DAC_SCK_AD2CFG = 1;
    DAC_LDAC_AD2CFG = 1;
    
    // set RD8, RB10, RB11, RB13 as output pins
    DAC_CS_TRIS = 0;
    DAC_SDI_TRIS = 0;
    DAC_SCK_TRIS = 0;
    DAC_LDAC_TRIS = 0;
    
    // set default state: CS=1, SCK=0, SDI=0, LDAC=1
    DAC_CS_PORT = 1;
    DAC_SCK_PORT = 0;
    DAC_SDI_PORT = 0;
    DAC_LDAC_PORT = 1;

}

uint16_t dac_convert_milli_volt(uint16_t milliVolt)
{
    if (milliVolt > 4095) milliVolt = 4095; //Vout = (AnalogVal / 4096) * 4096(Vref)
    return milliVolt;
}

void dac_send(uint16_t cmd, uint32_t interrupt_counter)
{
    uint16_t i;
    DAC_CS_PORT = 0;
    Nop();
        
    for (i = 0; i < 16; i++)
        {
        if (cmd & 0b1000000000000000)
            DAC_SDI_PORT = 1;
        else
            DAC_SDI_PORT = 0;
            Nop();
            DAC_SCK_PORT = 1;
            Nop();
            DAC_SCK_PORT = 0;
            Nop();

            cmd <<= 1;
        }
        DAC_CS_PORT = 1;
        Nop();

        DAC_LDAC_PORT = 0;
        Nop();
        Nop();
        DAC_LDAC_PORT = 1;
}
