#ifndef MY_ADC_H
#define MY_ADC_H
#include <zephyr/drivers/adc.h>

extern volatile struct adc_dt_spec adc_channel;
extern volatile int16_t adc_buf;
extern volatile struct adc_sequence sequence;
extern volatile int32_t bat_meas_mv;

int adc_initialize(void);
int measure_bat_mv(int32_t *mv_res);

#endif