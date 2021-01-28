#include <c8051F330.h>
#include "crc.h"
#include "define.h"
#include "type.h"
#include "flash.h"
#include "F350_FlashPrimitives.h"
#include "F350_FlashUtils.h"

sbit    r_t=P1^6; //������ ��������
/******************************************************************************
1.0					VARIABLES
******************************************************************************/
code baudrate br[8]={
{0x00,0x2B,0xe714,0xc5d9},	//9600        osc/12
{0x00,0x72,0xed4f,0xd463},	//14400       osc/12
{0x00,0x61,0xed4f,0xd463},	//19200       osc/12
{0x01,0x60,0xf9c5,0xf176},	//38400       osc/4
{0x01,0x93,0xF38B,0xE2ED},	//56000       osc/4
{0x01,0x96,0xF38B,0xE2ED},	//57600       osc/4
{0x08,0x2B,0xF38B,0xE2ED}	//115200      osc
};

//���������� �������� �������
modbus mb;

xdata union{
    unsigned char modbuf[modbufsize];
    struct{
        unsigned char address;
        unsigned char function;
        unsigned char exeption;        
    }exception;//����������� ��� �������� ��������

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
    }read_req;//������� �����

    struct{
        unsigned char address;
        unsigned char function;
        unsigned char byte_count;        
    }read_resp;//������� �������

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int register_address;
        unsigned int register_value;
    }write_req;//����� ������ ������� �����

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int register_address;
        unsigned int register_value;
    }write_resp;//����� ������ ������� �������

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
        unsigned char byte_count;
        unsigned int reg[24];
    }write_mult_req;//����� ������� �����

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
    }write_mult_resp;//����� ������� �������

}buf;

//������� �������
xdata union{
    unsigned int reg[37];
    struct{
        unsigned int sts;//������  
        unsigned int adc[2];//������� ��� ��� - unsigned int A[2]
        unsigned int ADC[3][2];//�������� ���   - unsigned int A[2]
        unsigned int ntc[2];//adc code for two ntc temperature channels cuted to 16 bits
        unsigned int cmd;//������� 
        unsigned int dacs;//values for DAC0 in low byte and DAC1 in hight byte                       FLASH BEGINS HERE
        unsigned int offset0[2];//�������
        unsigned int gain0[2];//���������
        unsigned int Kg0;//����.���������
        unsigned int K0;//������� ������ �� ����� �������� �����������
        unsigned int offset1[2];//�������
        unsigned int gain1[2];//���������
        unsigned int Kg1;//����.���������
        unsigned int K1;//������� ������ �� ����� �������� �����������
        unsigned int offset2[2];//�������
        unsigned int gain2[2];//���������
        unsigned int Kg2;//����.���������
        unsigned int K2;//������� ������ �� ����� �������� ����������� 
        unsigned int ntcOffset0;//adc offset for two ntc temperature channels
        unsigned int ntcGain0;//adc gain for two ntc temperature channels
        unsigned int ntcOffset1;//adc offset for two ntc temperature channels
        unsigned int ntcGain1;//adc gain for two ntc temperature channels
        unsigned int address;
        unsigned int baud;
    }r;
}mreg;


//������� ���������� ������ � �������
code w_a_v WAV[]={
{0,0},//sts;//������
{0,0},//������� ��� ���
{0,0},
{0,0},//�������� ���
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},//ntc
{0,0},
{0,65535},//&mreg.r.cmd,
{0,65535},//&mreg.r.dacs
{0,65535},//offset[0];
{0,65535},//offset[0];
{0,65535},//gain[0];
{0,65535},//gain[0];
{0,65535},//Kg;
{0,65535},//K;
{0,65535},//offset[0];
{0,65535},//offset[0];
{0,65535},//gain[0];
{0,65535},//gain[0];
{0,65535},//Kg;
{0,65535},//K;
{0,65535},//offset[0];
{0,65535},//offset[0];
{0,65535},//gain[0];
{0,65535},//gain[0];
{0,65535},//Kg;
{0,65535},//K;
{0,65535},//&mreg.r.ntcO+ffset0,
{0,65535},//&mreg.r.ntcGain0,
{0,65535},//&mreg.r.ntcOffset1,
{0,65535},//&mreg.r.ntcGain1,
{1,255},//&mreg.r.address,
{0,6},//&mreg.r.baud,
};

//���������� ��������� ������ � ��� ����������� ��������
struct{
    unsigned char tact;//0 ��� � th[0], 1 ��� � th[1], 2 crc_low, 3 crc_hi, 4 ��������� �� ������� 
    thread th[2];    
}u0;

/******************************************************************************
2.0					Predeclared functions
******************************************************************************/
void HandleReceiveInterrupt(void);
void HandleTransmitInterrupt(void);
void error();

/******************************************************************************
3.0					Functions itself
******************************************************************************/

void modbus_init()
{
    char i;
    char code * src;
    char xdata * dest;
    
    mreg.r.cmd = 0;
    //���������� �� ����
    src = (char*)0x1a00;
    dest = (char*)&mreg.r.dacs;
    for(i=0;i<sizeof(mreg)-24;i++){
        *dest++ = *src++;
    }

    mreg.r.address = 1;
    mreg.r.baud = 0;

    CKCON = br[(char)mreg.r.baud].ckcon;
    TH1 = br[(char)mreg.r.baud].th1;
    mb.crcHi=0xff; 
    mb.crcLo=0xff;
    mb.ptr = &buf.modbuf;
    REN0 = 1;
}

void crc(unsigned char next_one)
{
    unsigned char crc_index;

    crc_index = mb.crcLo ^ next_one;
    mb.crcLo = CRC_hi [crc_index] ^ mb.crcHi;
    mb.crcHi = CRC_lo [crc_index];
}

void uart_isr (void) interrupt 4 using 1
{
    if(TI0){
        TI0 = 0;
        HandleTransmitInterrupt();
    }
    if(RI0){
        RI0 = 0;
        HandleReceiveInterrupt();
    }
}

/******************************************************************************
4.0					Functions itself predeclared in section 2.0
******************************************************************************/

void HandleReceiveInterrupt(void)
{
  unsigned int time;

	TR0 = 0;
	time = br[(char)mreg.r.baud].time15;			//�������� ������ �� 1,5�������
  TH0 = time>>8; TL0 = time; 
  TR0 = 1;

	if(mb.ptr > (&buf.modbuf + modbufsize-1)) mb.ptr = &buf.modbuf + modbufsize-1;//���������� ������� ������� (����� �������� � 0 ������.)
	*mb.ptr = SBUF0;
	mb.ptr++;
  mb.counter++;                   //������������� ������� ��������� ����� ��� ������� ������������ ������
    
  crc(SBUF0);
}

/*****************************************************************************/

void HandleTransmitInterrupt(void)
{
    unsigned char byte;
    
    switch(u0.tact){
        case 0: //�������� ����� �� ���������
        case 1:
            byte = *u0.th[u0.tact].ptr;
            SBUF0 = byte;
            crc(byte);
            u0.th[u0.tact].ptr++;
            if(--u0.th[u0.tact].count == 0)u0.tact++;
            break;
        
        case 2:
            SBUF0 = mb.crcLo;
            u0.tact++;
            break;

        case 3:
            SBUF0 = mb.crcHi;
            u0.tact++;
            break;

        case 4:
            mb.counter = 0;         //�����������.
            mb.ptr = &buf.modbuf;       
    		mb.crcHi=0xff; 
    		mb.crcLo=0xff;        
            byte = 0; while(--byte); r_t = 1;
            REN0 = 1;               //��������� ������
            break;
    }
}

/*****************************************************************************/
void refresh_flash()
{
    unsigned char i;
    char code * src;
    char xdata * dest;

	PCA0MD &= ~(1<<6);//disable WD
	FLASH_Update (0x1a00, (char xdata*)&mreg.r.dacs, sizeof(mreg)-24);
    PCA0CPH2=0xFF;//reset watchdog
	PCA0MD |= 1<<6;//enable WD

    src = (char*)0x1a00;
    dest = (char*)&mreg.r.dacs;
    for(i=0;i<sizeof(mreg)-24;i++){
        *dest++ = *src++;
    }    
    /* �������� ���� ������� ����� */
    /*
    if(buf.write_req.register_address == (int)&mreg.r.baud){
    	CKCON = br[(char)mreg.r.baud].ckcon;
    	TH1 = br[(char)mreg.r.baud].th1;
    } 
    */                       
}

/*****************************************************************************/
bit flag_time35;
void timer_0 (void) interrupt 1 using 3
{ 
    unsigned int time;
    unsigned char i,off;
        
    TF0 = 0; TR0 = 0; REN0 = 0;
    
    if(flag_time35){
        //����� ������� � 3,5 ������
        flag_time35 = 0;
        if(buf.modbuf[0] == 0){
        	mb.ptr = &buf.modbuf;			//������� �������! � �����������.
        	mb.crcHi=0xff; 
        	mb.crcLo=0xff;
        	mb.counter = 0;
            i = 0; while(--i); r_t = 0;
            REN0 = 1;
            return; //������� ������ ������� �� ������
        }
        r_t = 0; i = 0; while(--i);
        i = 0; while(--i); r_t = 0;
        TI0 = 1;
        return;    
    }
                
    /* �������� ���������� ���� */
    if((mb.crcHi | mb.crcLo)){
		mb.ptr = &buf.modbuf;			//������� �������! � �����������.
		mb.crcHi=0xff; 
		mb.crcLo=0xff;
		mb.counter = 0;
        REN0 = 1;
        return;				
	}
    
    /* ������ ������������ ������ � �� ��������� ������ */
    if(mb.counter >= modbufsize && buf.modbuf[0] == *(((char*)&mreg.r.address)+1)){
        buf.exception.exeption = 4; //��������� �������� 
        error();
        return;    
    }

    /* ������� */
	if(buf.modbuf[0] == 0 || buf.modbuf[0] == *(((char*)&mreg.r.address)+1)){//�������� ������ 
        switch(buf.modbuf[1]){				//function code
            case 3:                     //������� ������ ��� ����� ������� (�� ������� ������� � ������� RAM)
                if(buf.read_req.quantity > (sizeof(mreg.reg)/sizeof(int))){
                    buf.exception.exeption = 3; //�������� �������
                    error();
                    return;
                }
                /* ���������� �������� ��������� */
                if(buf.read_req.start_address>(sizeof(mreg.reg)/sizeof(int))-1 || 
                buf.read_req.start_address + buf.read_req.quantity-1 > (sizeof(mreg.reg)/sizeof(int))-1){
                    buf.exception.exeption = 2; //������� ������ ��� ��������
                    error();
                    return;
                }
                /* ��������� ������ */
                u0.tact = 0;
                u0.th[0].count = 3;                         //���������
                u0.th[0].ptr = buf.modbuf;
                u0.th[1].ptr = (char*)&mreg.reg[buf.read_req.start_address];
                u0.th[1].count = buf.read_resp.byte_count = buf.read_req.quantity * 2; //���
                break;

            case 6:                     //����� � ������
                /* �������� ������ */
                if(buf.write_req.register_address > (sizeof(mreg.reg)/sizeof(int))-1){
                    buf.exception.exeption = 2; //������� ������ ��� ��������
                    error();
                    return;
                }
                if(WAV[buf.write_req.register_address].min ==0 && WAV[buf.write_req.register_address].max == 0){
                    buf.exception.exeption = 2; //������� ������ ��� ��������
                    error();
                    return;
                }
                

                if(buf.write_req.register_value >= WAV[buf.write_req.register_address].min && 
                    buf.write_req.register_value <= WAV[buf.write_req.register_address].max){
                    
                    /* ����� � ������ */
                    mreg.reg[buf.write_req.register_address] = buf.write_req.register_value;
                    /* ������� ������� */
                    buf.write_resp.register_value = mreg.reg[buf.write_req.register_address];                            
                    
                    /* ���������� ������ ������ � ������, ��� � ������ ������� ������������ ���� � ���� */
                    if(buf.write_req.register_address >= &mreg.r.dacs-&mreg){//���� �� ������������ ����
                        refresh_flash();                        
                    }
					
                    /* ��������� ������ */
                    u0.tact = 0;
                    u0.th[0].count = 4;                         //���������
                    u0.th[0].ptr = buf.modbuf;
                    u0.th[1].count = 2; //���
                    u0.th[1].ptr = (char pdata*)&buf.write_resp.register_value;
                    break;
                }
                else{
                    buf.exception.exeption = 3; //������� ��������
                    error();
                    return;
                }

                break;

			case 16:
                /* �������� ������ */
                if(buf.read_req.quantity > (sizeof(mreg.reg)/sizeof(int))){
                    buf.exception.exeption = 3; //�������� �������
                    error();
                    return;
                }
                /* ���������� �������� ��������� */
                if(buf.write_mult_req.start_address>(sizeof(mreg.reg)/sizeof(int))-1 || 
                buf.write_mult_req.start_address + buf.write_mult_req.quantity-1 > (sizeof(mreg.reg)/sizeof(int))-1){
                    buf.exception.exeption = 2; //������� ������ ��� ��������
                    error();
                    return;
                }
				//����²��� �������� �������
                off = 0;//��� ���� �� ������ �������
                for(i=buf.write_mult_req.start_address;i<buf.write_mult_req.start_address + buf.write_mult_req.quantity;i++){
                    if(WAV[i].min ==0 && WAV[i].max == 0){
                        buf.exception.exeption = 2; //������� ������ ��� ��������
                        error();
                        return;
                    }
                    if(buf.write_mult_req.reg[off] < WAV[i].min || buf.write_mult_req.reg[off] > WAV[i].max){
                        buf.exception.exeption = 3; //������� ��������
                        error();
                        return;
                    }
                    off++;
                }

                //���������
                off = 0;
                for(i=buf.write_mult_req.start_address;i<buf.write_mult_req.start_address + buf.write_mult_req.quantity;i++){
                    mreg.reg[i] = buf.write_mult_req.reg[off++];
                }

                /* ��������� �� ��������� � ������� ���� ����� �������� � ���� */
                if(buf.write_mult_req.start_address + buf.write_mult_req.quantity > (&mreg.r.dacs-&mreg)){
                    refresh_flash();
                }
                                
				/* ��������� ������ */
                u0.tact = 0;
                u0.th[0].count = 4;                         //���������
                u0.th[0].ptr = buf.modbuf;
                u0.th[1].count = 2; //���
                u0.th[1].ptr = (char pdata*)&buf.write_mult_resp.quantity;				
				break;



                default:
                    /* ������� �� ����������� */
                    buf.exception.exeption = 1;
                    error();
                    break;
        }

        flag_time35 = 1;
        time = br[(char)mreg.r.baud].time35;			//�������� ������ �� 3,5�������
        TH0 = time>>8; TL0 = time; 
        TR0 = 1;

        mb.crcHi=0xff; 
        mb.crcLo=0xff;
                
	}
	else{
		mb.ptr = &buf.modbuf;			
		mb.crcHi=0xff; 
		mb.crcLo=0xff;
        mb.counter = 0;
        REN0 = 1;
	}

}

void error()
{
    unsigned int time;

	mb.ptr = &buf.modbuf;			
	buf.exception.function |= 0x80;       

    u0.tact = 1;        //��� �� � ���e� ���� ����������� ������
    u0.th[1].count = 3; //������ ������� ��� �������
    u0.th[1].ptr = &buf.modbuf;
    
    mb.crcHi=0xff; 
	mb.crcLo=0xff;
    mb.counter = 0;
    
    flag_time35 = 1;
    time = br[(char)mreg.r.baud].time35;			//�������� ������ �� 3,5�������
    TH0 = time>>8; TL0 = time; 
    TR0 = 1;    
}