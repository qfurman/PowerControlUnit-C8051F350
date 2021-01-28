

extern code baudrate br[];

extern modbus mb;

extern xdata union{
    unsigned char modbuf[modbufsize];
    struct{
        unsigned char address;
        unsigned char function;
        unsigned char exeption;        
    }exception;//повідомлення про виключну ситуацію

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
    }read_req;//читання Запит

    struct{
        unsigned char address;
        unsigned char function;
        unsigned char byte_count;        
    }read_resp;//читання відповідь

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int register_address;
        unsigned int register_value;
    }write_req;//запис одного регістра Запит

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int register_address;
        unsigned int register_value;
    }write_resp;//запис одного регістра відповідь

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
        unsigned char byte_count;
        unsigned int reg[7];
    }write_mult_req;//запис регістрів Запит

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
    }write_mult_resp;//запис регістрів відповідь

}buf;


//регістри модбаса

extern xdata union{
    unsigned int reg[41];
    struct{
        unsigned int sts;//статус  
        unsigned int adc[2];//швидкий код ацп - unsigned int A[2]
        unsigned int ADC[3][2];//повільний код   - unsigned int A[2]
        unsigned int ntc[2];//adc code for two ntc temperature channels cuted to 16 bits
        unsigned int cmd;//команда 
        unsigned int dacs;//values for DAC0 in low byte and DAC1 in hight byte 
        unsigned int offset0[2];//зміщення
        unsigned int gain0[2];//підсилення
        unsigned int Kg0;//коеф.підсилення
        unsigned int K0;//кількість вибірок за якими робиться усереднення
        unsigned int offset1[2];//зміщення
        unsigned int gain1[2];//підсилення
        unsigned int Kg1;//коеф.підсилення
        unsigned int K1;//кількість вибірок за якими робиться усереднення
        unsigned int offset2[2];//зміщення
        unsigned int gain2[2];//підсилення
        unsigned int Kg2;//коеф.підсилення
        unsigned int K2;//кількість вибірок за якими робиться усереднення або в даному випадку пропустити канал якщо 1
        unsigned int ntcOffset0;//adc offset for two ntc temperature channels
        unsigned int ntcGain0;//adc gain for two ntc temperature channels
        unsigned int ntcOffset1;//adc offset for two ntc temperature channels
        unsigned int ntcGain1;//adc gain for two ntc temperature channels
        unsigned int address;
        unsigned int baud;
    }r;
}mreg;

void modbus_init();
