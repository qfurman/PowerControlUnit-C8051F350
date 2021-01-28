    
extern xdata unsigned int xtimer_arr[];

extern code struct virtual_timer timer_arr[];
extern bit AC_on;//Мережа присутня

void start_timeout();
void end_timeout();
void end_timeout1s();
void start_timeout1s(unsigned int i);
void start_T_0_1sV();
void end_T_0_1sV();
void stop_meo();
void restartI2C();

extern int cntr[];




