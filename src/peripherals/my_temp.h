#ifndef MY_TEMP_H
#define MY_TEMP_H
#include <zephyr/kernel.h>
#include <nrfx_temp.h>

extern volatile float chip_temp;

int init_temp_peripheral(void);
int get_temperature(float *temp);



#endif