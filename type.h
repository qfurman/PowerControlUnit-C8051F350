
//віртуальний таймер
struct virtual_timer{
    unsigned int xdata*tick;                         //відліки до нуля
    void(*start_funct)(unsigned int i);  //фунція запуску
    void(*end_funct)();    //дія після досягнення нуля
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

/* структура для організації швидкостей обміну і таймаутів */
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
    unsigned char xdata * ptr;  //вказує на джерело або призначення
    unsigned char counter;    //лічильник прийнятих/переданих байтів
};
typedef struct modbus modbus;

/* організація передачі - кількість байт, джерело*/
struct thread{
    unsigned char count;
    unsigned char xdata*ptr;
};
typedef struct thread thread;

/* діапазон значень, якщо обидва нульові то записзаборонено*/
struct write_address_value{
    unsigned int min;   //min. значення
    unsigned int max;   //max. значення
};
typedef struct write_address_value w_a_v;


