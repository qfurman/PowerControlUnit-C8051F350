#include <c8051F350.h>

unsigned char i2c_op, i2c_addr;
unsigned int i2c_data;

/******************************************************************************/
void byte_write();
void cur_addr_read();
void random_read();
/******************************************************************************/

//i2c_op  
/*
0 write
1 current addres read
2 random read;
*/
void I2C_isr() interrupt 7 using 1
{
    switch(i2c_op & 0x0f)
    {
        case 0x00: //write         
            byte_write();
            break;

        case 0x01: //current read
            cur_addr_read();
            break;     

        case 0x02: //random read
            random_read();
            break;
        default:
            i2c_op = 0x80;
    }
    SI = 0;
}
/******************************************************************************/
void byte_write()
{
    switch(i2c_op & 0xf0)
    {
        case 0x00: //device addres         
            STA = 0;
            STO = 0;
            SMB0DAT = 0x48;
            i2c_op |= 0x10;
            break;

        case 0x10:  //word adress
            STA = 0;
            STO = 0;
            SMB0DAT = i2c_addr;
            i2c_op |= 0x20;
            break;

        case 0x30:  //data
            STA = 0;
            STO = 0;
            SMB0DAT = i2c_data;
            i2c_op |= 0x40;
            break;

        case 0x70:  //stop
            STA = 0;
            STO = 1;
            i2c_op |= 0x80;
            break;
    }
}
/******************************************************************************/
void cur_addr_read()
{
    switch(i2c_op & 0xf0)
    {
        case 0x00: //device addres         
            STA = 0;
            STO = 0;
            SMB0DAT = 0x91;
            i2c_op |= 0x10;
            break;

        case 0x10:  //data
            STA = 0;
            STO = 0;
            ACK = 1;
            i2c_op |= 0x20;
            break;

        case 0x30:  //data
            STA = 0;
            STO = 0;
            ACK = 1;
            i2c_op |= 0x40;
            *((char*)&i2c_data) = SMB0DAT;
            break;

        case 0x70:  //stop
            STA = 0;
            STO = 1;
            ACK = 0;
            i2c_op |= 0x80;
            *((char*)&i2c_data + 1) = SMB0DAT;
            break;
    }    
}
/******************************************************************************/
void random_read()
{
    switch(i2c_op & 0xf0)
    {
        case 0x00: //device addres         
            STA = 0;
            STO = 0;
            SMB0DAT = 0x48;
            i2c_op |= 0x10;
            break;

        case 0x10:  //word adress
            STA = 0;
            STO = 0;
            SMB0DAT = i2c_addr;
            i2c_op |= 0x20;
            break;

        case 0x30:  //restart
            //заміна команди на cur_addr_read
            STA = 1;
            STO = 0;
            i2c_op = 0x01;
            break;
    }    
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
