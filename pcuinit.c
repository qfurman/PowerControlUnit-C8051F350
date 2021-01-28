/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "c8051f350.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA_Init()
{
    PCA0CN    = 0x40;
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
    PCA0CPL2  = 0xFF;
    PCA0MD    |= 0x40;
}

void Timer_Init()
{
    TCON      = 0x45;
    TMOD      = 0x21;
    CKCON     = 0x01;
    TH1       = 0x96;
    TMR2CN    = 0x04;
    TMR2RLL   = 0x7F;
    TMR2RLH   = 0x60;
    TMR2L     = 0x7F;
    TMR2H     = 0x60;
}

void UART_Init()
{
    SCON0     = 0x30;
}

void SMBus_Init()
{
    SMB0CF    = 0xC1;
}

void SPI_Init()
{
    SPI0CN    = 0x05;
}

void ADC_Init()
{
    ADC0CN    = 0x00;
    ADC0CLK   = 0x13;
    ADC0DECL  = 0x79;
    ADC0MUX   = 0x01;
}

void DAC_Init()
{
    IDA0CN    = 0xF2;
    IDA1CN    = 0xF2;
}

void Voltage_Reference_Init()
{
    REF0CN    = 0x03;
}

void Port_IO_Init()
{
    // P0.0  -  Unassigned,  Open-Drain, Digital
    // P0.1  -  Unassigned,  Open-Drain, Digital
    // P0.2  -  Unassigned,  Open-Drain, Digital
    // P0.3  -  Unassigned,  Open-Drain, Digital
    // P0.4  -  TX0 (UART0), Open-Drain, Digital
    // P0.5  -  RX0 (UART0), Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Open-Drain, Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Skipped,     Open-Drain, Analog
    // P1.7  -  Skipped,     Open-Drain, Analog

    P1MDIN    = 0x3F;
    P1SKIP    = 0xC0;
    XBR0      = 0x01;
    XBR1      = 0x40;
}

void Oscillator_Init()
{
    char i;
    FLSCL     = 0x10;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x02;
    OSCICN    = 0x83;
}

void Interrupts_Init()
{
    EIE1      = 0x88;
    IE        = 0xB2;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
    Timer_Init();
    UART_Init();
    SMBus_Init();
    SPI_Init();
    ADC_Init();
    DAC_Init();
    Voltage_Reference_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
