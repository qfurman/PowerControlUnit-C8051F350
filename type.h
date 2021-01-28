
//���������� ������
struct virtual_timer{
    unsigned int xdata*tick;                         //����� �� ����
    void(*start_funct)(unsigned int i);  //������ �������
    void(*end_funct)();    //�� ���� ���������� ����
};
typedef struct virtual_timer virtual_timer;

struct ADC{
    unsigned long Code;
    unsigned char index;
    unsigned char size;
};
typedef struct ADC ADC;

struct ADCBUF{
    unsigned char adch;
    unsigned char adcm;
    unsigned char adcl;
};
typedef struct ADCBUF ADCBUF;

/* ��������� ��� ���������� ���������� ����� � �������� */
struct baudrate{
	unsigned char ckcon;
	unsigned char th1;
	unsigned int time15;
	unsigned int time35;
};
typedef struct baudrate baudrate;

struct modbus{
    unsigned char crcLo;
    unsigned char crcHi;
    unsigned char xdata * ptr;  //����� �� ������� ��� �����������
    unsigned char counter;    //�������� ���������/��������� �����
};
typedef struct modbus modbus;

/* ���������� �������� - ������� ����, �������*/
struct thread{
    unsigned char count;
    unsigned char xdata*ptr;
};
typedef struct thread thread;

/* ������� �������, ���� ������ ������ �� ���������������*/
struct write_address_value{
    unsigned int min;   //min. ��������
    unsigned int max;   //max. ��������
};
typedef struct write_address_value w_a_v;


