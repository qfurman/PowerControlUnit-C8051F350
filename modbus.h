

extern code baudrate br[];

extern modbus mb;

extern xdata union{
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
        unsigned int reg[7];
    }write_mult_req;//����� ������� �����

    struct{
        unsigned char address;
        unsigned char function;
        unsigned int start_address;
        unsigned int quantity;
    }write_mult_resp;//����� ������� �������

}buf;


//������� �������

extern xdata union{
    unsigned int reg[41];
    struct{
        unsigned int sts;//������  
        unsigned int adc[2];//������� ��� ��� - unsigned int A[2]
        unsigned int ADC[3][2];//�������� ���   - unsigned int A[2]
        unsigned int ntc[2];//adc code for two ntc temperature channels cuted to 16 bits
        unsigned int cmd;//������� 
        unsigned int dacs;//values for DAC0 in low byte and DAC1 in hight byte 
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
        unsigned int K2;//������� ������ �� ����� �������� ����������� ��� � ������ ������� ���������� ����� ���� 1
        unsigned int ntcOffset0;//adc offset for two ntc temperature channels
        unsigned int ntcGain0;//adc gain for two ntc temperature channels
        unsigned int ntcOffset1;//adc offset for two ntc temperature channels
        unsigned int ntcGain1;//adc gain for two ntc temperature channels
        unsigned int address;
        unsigned int baud;
    }r;
}mreg;

void modbus_init();
