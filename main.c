#include "c8051f350.h"
#include <intrins.h>
#include <include.h>
#include "define.h"
#include "type.h"
#include "adc.h"
#include "timer_2.h"
#include "F350_FlashUtils.h"
#include "F350_FlashPrimitives.h"
#include "modbus.h"
#include "i2c.h"

/******************************************************************************/
void Init_Device(void);
char testADCparam();
void refreshADC();
void wr_flash();
void refresh();
void refresh_modbus_ADC();
/******************************************************************************/
void main()
{
  //char key_count;

  Init_Device();
  PCA0CPH2=0xFF;
  modbus_init();

  //refreshADC();
  ADC0MD = 0;//stop ADC
  ADC0MUX = 0x01;//channel 0 connector X1
  ADC0CN =  mreg.r.Kg0 & 0x07;//Programmable Gain
  ADC0CGH = mreg.r.gain0[1]; ADC0CGM = (char)(mreg.r.gain0[0]>>8); ADC0CGL = (char)mreg.r.gain0[0];
  ADC0COH = mreg.r.offset0[1]; ADC0COM = (char)(mreg.r.offset0[0]>>8); ADC0COL = (char)(mreg.r.offset0[0]);
  ADC0MD = 0x83;//запусити АЦП 

  for(;;){
    
    PCA0CPH2=0xFF;
    
    IDA0 = mreg.r.dacs >> 8;
    IDA1 = mreg.r.dacs;

    adc_mean();

    mreg.r.sts =  0;
    mreg.r.adc[0] = 0;
    mreg.r.adc[1] = 0;
    mreg.r.ADC[0][0] = adcResult[0]>>16;
    mreg.r.ADC[0][1] = adcResult[0];
    mreg.r.ADC[1][0] = adcResult[1]>>16;
    mreg.r.ADC[1][1] = adcResult[1];      
    mreg.r.ADC[2][0] = adcResult[2]>>16;
    mreg.r.ADC[2][1] = adcResult[2];
    mreg.r.ntc[0] = ntcResult[0];
    mreg.r.ntc[1] = ntcResult[1];
    
    if(testADCparam()) refreshADC();//перевірка чи змінились параметри АЦП

    switch(mreg.r.cmd){
      case 0: break;
//      case 1: 
//        mreg.r.cmd = 0;
//        AD0CALC = 0;// зняти біт ознаки кінця калібрування
//        EIE1 &= ~0x08;//EADC0 = 0;
//        //offset and GAIN calibration
//        ADC0MD = 0x81;
//        do PCA0CPH2=0xFF;
//        while(!AD0CALC);
//        
//        refresh_modbus_ADC();//переписати результати в регістри модбасу 
//        ADC0MD = 0x83;
//        EIE1 |= 0x08;//EADC0 = 1;
//        
//        break;
    }
    
    refresh();

    PCA0CPH2=0xFF;//reset watchdog
  }
}
/******************************************************************************/
//перевірка чи змінились параметри АЦП 

char testADCparam()
{
    /*
    if(ADC0COH != (char)mreg.r.offset[0]) return 1;
    if(ADC0COM != mreg.r.offset[1]>>8) return 1;
    if(ADC0COL != (char)mreg.r.offset[1]) return 1;

    if(ADC0CGH != (char)mreg.r.gain[0]) return 1;
    if(ADC0CGM != mreg.r.gain[1]>>8) return 1;
    if(ADC0CGL != (char)mreg.r.gain[1]) return 1;

    if(ADC0CN != mreg.r.Kg) return 1;
     */
    //adc.size = mreg.r.K;
    
    return 0;
}
/******************************************************************************/
//заміна коефіцієнтів АЦП 

void refreshADC()
{
    ADC0MD = 0x00;//зупинити АЦП
    /*
    ADC0COH = mreg.r.offset[0];  
    ADC0COM = mreg.r.offset[1]>>8;
    ADC0COL = (char)mreg.r.offset[1];

    ADC0CGH = mreg.r.gain[0];
    ADC0CGM = mreg.r.gain[1]>>8;
    ADC0CGL = (char)mreg.r.gain[1];

    ADC0CN &= ~0x7;//обнулити біти підсилення
    ADC0CN |= mreg.r.Kg &0x07;//відсікти і вставити підсилення
    */
    //adc.size = mreg.r.K;

    ADC0MD = 0x83;//запусити АЦП 
}
/******************************************************************************/
//оновлення коефіцієнтів АЦП в регістрах модбасу, після калібрування 
/*void refresh_modbus_ADC()
{
    
    mreg.r.offset[0] = ADC0COH;  
    mreg.r.offset[1] = (unsigned int)ADC0COM<<8 | ADC0COL;

    mreg.r.gain[0] = ADC0CGH;
    mreg.r.gain[1] = (unsigned int)ADC0CGM<<8 | ADC0CGL;
    
    mreg.r.Kg = ADC0CN;//відсікти і вставити підсилення
    
}
*/
/******************************************************************************/
/*
void wr_flash()
{
   
    //записати у флеш
	PCA0MD &= ~(1<<6);//disable WD
	FLASH_Update (0x1a00, (char xdata*)&spiRX, sizeof(SPI_RX));
    PCA0CPH2=0xFF;//reset watchdog
	PCA0MD |= 1<<6;//enable WD
   
}
*/
/******************************************************************************/
//оновлення параметрів в буфері для відправки
void refresh()
{

    /*
    in.in.adc[0] = ADC0H;
    in.in.adc[1] = (unsigned int)ADC0M<<8 | ADC0L;
    */
    /*
    mreg.r.ADC[0] = adc.Code>>16;
    mreg.r.ADC[1] = adc.Code;
    */
         
}
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/



