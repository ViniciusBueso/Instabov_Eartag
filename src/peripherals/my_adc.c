#include "my_adc.h"

volatile struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

volatile struct adc_dt_spec adc_channel;
volatile int16_t adc_buf;
volatile struct adc_sequence sequence = {
		.buffer = &adc_buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(adc_buf),
		//Optional
		//.calibrate = true,
};

volatile int32_t bat_meas_mv = 0;


int adc_initialize(void){
	int err;

	if(!adc_is_ready_dt(&adc_channel)){
		return -ENODEV;
	}

	err = adc_channel_setup_dt(&adc_channel);
	if(err<0){
		return err;
	}

	err = adc_sequence_init_dt(&adc_channel, &sequence);
	if(err<0){
		return err;
	}
}


int measure_bat_mv(int32_t *mv_res){
	int err;
	err = adc_read(adc_channel.dev, &sequence);
	if(err){
		return err;
	}
	*mv_res = adc_buf;
	err = adc_raw_to_millivolts_dt(&adc_channel, mv_res);
	if(err){
		return err;
	}
}