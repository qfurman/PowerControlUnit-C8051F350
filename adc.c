#include <c8051f350.h>
#include <include.h>
#include "define.h"
#include "type.h"
#include "modbus.h"
#include "timer_2.h"

//ADC adc;

void adc_meen();
//xdata unsigned int adc_buf[16] _at_ 0x00;
xdata ADCBUF adcbuf[3][16];
unsigned long adcResult[3];
xdata unsigned int ntcbuf[2][8];
unsigned int ntcResult[2];
unsigned char adcCH;//current ADC input
unsigned char adcCntr;//samples counter
bit x1F, x2F, x3F, ntc1F, ntc2F;
/******************************************************************************/
void adc_isr() interrupt 10 using 2
{
  unsigned char i;

  AD0INT = 0;
  
  switch(adcCH){
    //X1 input
    case 0:
      if(adcCntr > 15) {adcCntr++; break;}//rid of   first two result
      adcbuf[0][adcCntr].adcl = ADC0L;
      adcbuf[0][adcCntr].adcm = ADC0M;
      adcbuf[0][adcCntr].adch = ADC0H;
      adcCntr = ++adcCntr & 0xf;
      if(adcCntr == 0){
        //switch to next channel
        x1F = 1;
        if (mreg.r.cmd == 1){
          ADC0CN =  mreg.r.Kg0 & 0x07;//Programmable Gain
          i = mreg.r.gain0[0];              if ( ADC0CGH != i ) {ADC0MD = 0; ADC0CGH = i;}//stop ADC and update regs
          i = (char)(mreg.r.gain0[1] >> 8); if ( ADC0CGM != i ) {ADC0MD = 0; ADC0CGM = i;}
          i = (char)mreg.r.gain0[1];        if ( ADC0CGL != i ) {ADC0MD = 0; ADC0CGL = i;}
          i = mreg.r.offset0[0];            if ( ADC0COH != i ) {ADC0MD = 0; ADC0COH = i;}
          i = (char)(mreg.r.offset0[1]>>8); if ( ADC0COM != i ) {ADC0MD = 0; ADC0COM = i;}
          i = (char)(mreg.r.offset0[1]);    if ( ADC0COL != i ) {ADC0MD = 0; ADC0COL = i;}
          if (ADC0MD == 0) ADC0MD = 0x83;//запусити АЦП
          adcCntr = 0xfe;//rid of first two result           
          break;//prohibit to switch next
        }
        adcCntr = 0xfe;
        adcCH = 1;
        ADC0MD = 0;//stop ADC
        ADC0MUX = 0x23;//channel 2 connector X2
        ADC0CN =  mreg.r.Kg1 & 0x07;//Programmable Gain
        ADC0CGH = mreg.r.gain1[0]; ADC0CGM = (char)(mreg.r.gain1[1]>>8); ADC0CGL = (char)mreg.r.gain1[1];
        ADC0COH = mreg.r.offset1[0]; ADC0COM = (char)(mreg.r.offset1[1]>>8); ADC0COL = (char)(mreg.r.offset1[1]);
        ADC0MD = 0x83;//запусити АЦП 
      }    
    break;
    //X2 input
    case 1:
      if(adcCntr > 15) {adcCntr++; break;}
      adcbuf[1][adcCntr].adcl = ADC0L;
      adcbuf[1][adcCntr].adcm = ADC0M;
      adcbuf[1][adcCntr].adch = ADC0H;
      adcCntr = ++adcCntr & 0xf;
      if(adcCntr == 0){
        //switch to next channel
        x2F = 1;
        if (mreg.r.cmd == 2){
          ADC0CN =  mreg.r.Kg1 & 0x07;//Programmable Gain
          i = mreg.r.gain1[0];              if ( ADC0CGH != i ) {ADC0MD = 0; ADC0CGH = i;}//stop ADC and update regs
          i = (char)(mreg.r.gain1[1] >> 8); if ( ADC0CGM != i ) {ADC0MD = 0; ADC0CGM = i;}
          i = (char)mreg.r.gain1[1];        if ( ADC0CGL != i ) {ADC0MD = 0; ADC0CGL = i;}
          i = mreg.r.offset1[0];            if ( ADC0COH != i ) {ADC0MD = 0; ADC0COH = i;}
          i = (char)(mreg.r.offset1[1]>>8); if ( ADC0COM != i ) {ADC0MD = 0; ADC0COM = i;}
          i = (char)(mreg.r.offset1[1]);    if ( ADC0COL != i ) {ADC0MD = 0; ADC0COL = i;}
          if (ADC0MD == 0) ADC0MD = 0x83;//запусити АЦП
          adcCntr = 0xfe;//rid of first two result 
          break;//prohibit to switch next
        }
        adcCntr = 0xfe;
        ADC0MD = 0;//stop ADC
        if (mreg.r.K2 == 0){
          adcCH = 2;
          ADC0MUX = 0x45;//channel 3 connector X3
          ADC0CN =  mreg.r.Kg2 & 0x07;//Programmable Gain
          ADC0CGH = mreg.r.gain2[0]; ADC0CGM = (char)(mreg.r.gain2[1]>>8); ADC0CGL = (char)mreg.r.gain2[1];
          ADC0COH = mreg.r.offset2[0]; ADC0COM = (char)(mreg.r.offset2[1]>>8); ADC0COL = (char)(mreg.r.offset2[1]);
        }else{
          adcCH = 3;//jump to ntc
          ADC0MUX = 0x68;//channel AIN6 NTC1
          ADC0CN =  0x0;//Programmable Gain write manualy
          ADC0CGH = (char)(mreg.r.ntcGain0 >> 8); ADC0CGM = (char)(mreg.r.ntcGain0); ADC0CGL = 0;
          ADC0COH = (char)(mreg.r.ntcOffset0 >> 8); ADC0COM = (char)(mreg.r.ntcOffset0); ADC0COL = 0;
        }
        ADC0MD = 0x83;//запусити АЦП 
      }    
    break;
    //X3 input
    case 2:
      if(adcCntr > 15) {adcCntr++; break;}
      adcbuf[2][adcCntr].adcl = ADC0L;
      adcbuf[2][adcCntr].adcm = ADC0M;
      adcbuf[2][adcCntr].adch = ADC0H;
      adcCntr = ++adcCntr & 0xF;
      if(adcCntr == 0){
        //switch to next channel
        x3F = 1;
        if (mreg.r.cmd == 3){
          ADC0CN =  mreg.r.Kg2 & 0x07;//Programmable Gain
          i = mreg.r.gain2[0];              if ( ADC0CGH != i ) {ADC0MD = 0; ADC0CGH = i;}//stop ADC and update regs
          i = (char)(mreg.r.gain2[1] >> 8); if ( ADC0CGM != i ) {ADC0MD = 0; ADC0CGM = i;}
          i = (char)mreg.r.gain2[1];        if ( ADC0CGL != i ) {ADC0MD = 0; ADC0CGL = i;}
          i = mreg.r.offset2[0];            if ( ADC0COH != i ) {ADC0MD = 0; ADC0COH = i;}
          i = (char)(mreg.r.offset2[1]>>8); if ( ADC0COM != i ) {ADC0MD = 0; ADC0COM = i;}
          i = (char)(mreg.r.offset2[1]);    if ( ADC0COL != i ) {ADC0MD = 0; ADC0COL = i;}
          if (ADC0MD == 0) ADC0MD = 0x83;//запусити АЦП
          adcCntr = 0xfe;//get rid of first two result           
          break;//prohibit to switch next
        }
        adcCntr = 0xfe;
        adcCH = 3;
        ADC0MD = 0;//stop ADC
        ADC0MUX = 0x68;//channel AIN6 NTC1
        ADC0CN =  0x0;//Programmable Gain write manualy
        ADC0CGH = (char)(mreg.r.ntcGain0 >> 8); ADC0CGM = (char)(mreg.r.ntcGain0); ADC0CGL = 0;
        ADC0COH = (char)(mreg.r.ntcOffset0 >> 8); ADC0COM = (char)(mreg.r.ntcOffset0); ADC0COL = 0;
        ADC0MD = 0x83;//запусити АЦП 
      }    
    break;
    //AIN6 NTC1 input
    case 3:
      if(adcCntr > 15) {adcCntr++; break;}
      ntcbuf[0][adcCntr] = (unsigned int) ADC0H<<8; 
      ntcbuf[0][adcCntr] |= ADC0M;
      adcCntr = ++adcCntr & 0x7;
      if(adcCntr == 0){
        //switch to next channel
        ntc1F = 1;
        if (mreg.r.cmd == 4){
          i = (char)(mreg.r.ntcGain0 >> 8);   if ( ADC0CGH != i ) {ADC0MD = 0; ADC0CGH = i;}//stop ADC and update regs
          i = (char)(mreg.r.ntcGain0);        if ( ADC0CGM != i ) {ADC0MD = 0; ADC0CGM = i;}
          i = (char)(mreg.r.ntcOffset0 >> 8); if ( ADC0COH != i ) {ADC0MD = 0; ADC0COH = i;}
          i = (char)(mreg.r.ntcOffset0);      if ( ADC0COM != i ) {ADC0MD = 0; ADC0COM = i;}
          if (ADC0MD == 0) ADC0MD = 0x83;//запусити АЦП
          adcCntr = 0xfe;//rid of first two result          
          break;//prohibit to switch next
        }
        adcCntr = 0xfe;
        adcCH = 4;
        ADC0MD = 0;//stop ADC
        ADC0MUX = 0x78;//channel AIN7 NTC1
        ADC0CN = 0x0;//Programmable Gain write manualy
        ADC0CGH = (char)(mreg.r.ntcGain1 >> 8); ADC0CGM = (char)(mreg.r.ntcGain1); ADC0CGL = 0;
        ADC0COH = (char)(mreg.r.ntcOffset1 >> 8); ADC0COM = (char)(mreg.r.ntcOffset1); ADC0COL = 0;
        ADC0MD = 0x83;//запусити АЦП 
      }    
    break;
    //AIN7 NTC2 input
    case 4:
      if(adcCntr > 15) {adcCntr++; break;}
      ntcbuf[1][adcCntr] = (unsigned int) ADC0H<<8; 
      ntcbuf[1][adcCntr] |= ADC0M;
      adcCntr = ++adcCntr & 0x7;
      if(adcCntr == 0){
        //switch to next channel
        ntc2F = 1;
        if (mreg.r.cmd == 5){
          i = (char)(mreg.r.ntcGain1 >> 8);   if ( ADC0CGH != i ) {ADC0MD = 0; ADC0CGH = i;}//stop ADC and update regs
          i = (char)(mreg.r.ntcGain1);        if ( ADC0CGM != i ) {ADC0MD = 0; ADC0CGM = i;}
          i = (char)(mreg.r.ntcOffset1 >> 8); if ( ADC0COH != i ) {ADC0MD = 0; ADC0COH = i;}
          i = (char)(mreg.r.ntcOffset1);      if ( ADC0COM != i ) {ADC0MD = 0; ADC0COM = i;}
          if (ADC0MD == 0) ADC0MD = 0x83;//запусити АЦП
          adcCntr = 0xfe;//rid of first two result   
          break;//prohibit to switch next
        }
        adcCntr = 0xfe;
        adcCH = 0;
        ADC0MD = 0;//stop ADC
        ADC0MUX = 0x01;//channel 0 connector X1
        ADC0CN = mreg.r.Kg0 & 0x07;//Programmable Gain
        ADC0CGH = mreg.r.gain0[0]; ADC0CGM = (char)(mreg.r.gain0[1]>>8); ADC0CGL = (char)mreg.r.gain0[1];
        ADC0COH = mreg.r.offset0[0]; ADC0COM = (char)(mreg.r.offset0[1]>>8); ADC0COL = (char)(mreg.r.offset0[1]);
        ADC0MD = 0x83;//запусити АЦП 
      }    
    break;
  }

}

/******************************************************************************/
char meanStg;

void adc_mean()
{
  char i, k;
  unsigned long temp, adcword;

  //if(adc.size > sizeof(adcbuf)/3)adc.size = sizeof(adcbuf)/3;
  switch (meanStg){
    case 0:
      if (x1F) {k = 0; meanStg = 1;}
      if (x2F) {k = 1; meanStg = 1;}
      if (x3F) {k = 2; meanStg = 1;}
      if (ntc1F) {k = 0; meanStg = 2;}
      if (ntc2F) {k = 1; meanStg = 2;}
    break;
      
    case 1:
      x1F = 0;
      x2F = 0;
      x3F = 0;
      temp = 0;
      for ( i=0; i<16; i++ ){
        adcword = adcbuf[k][i].adcl;
        *((char*)&adcword+2) = adcbuf[k][i].adcm;
        *((char*)&adcword+1) = adcbuf[k][i].adch;
        temp += adcword;
      }
      adcResult [k] = temp >> 4;      
      meanStg =0;
      break;
      
    case 2:
      ntc1F = 0;
      ntc2F = 0;
      temp = 0;
      for ( i=0; i<8; i++ ){
        temp += ntcbuf [k][i];
      }
      ntcResult [k] = temp >> 3;      
      meanStg =0;
      break;       
  }
  
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*void int0_isr() interrupt 0 using 1
{

}
*/
/******************************************************************************/
void timer3_ISR (void) interrupt 14 using 1
{
    
    TMR3CN = TMR3CN & 0x39; //TF3H = 0; TF3L = 0  TR3 = 0

}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/




