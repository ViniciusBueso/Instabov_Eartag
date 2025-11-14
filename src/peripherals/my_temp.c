#include "my_temp.h"


volatile float chip_temp = 300;


int init_temp_peripheral(void){
    nrfx_err_t status;
    nrfx_temp_config_t config = NRFX_TEMP_DEFAULT_CONFIG;
    status = nrfx_temp_init(&config, NULL);
    return status;
}


int get_temperature(float *temp){
    nrfx_err_t status;
    status = nrfx_temp_measure();
    if(status!=NRFX_SUCCESS){
        return status;
    }
    int32_t temperature = nrfx_temp_result_get();
    temperature = nrfx_temp_calculate(temperature);
    *temp = ((float)temperature)/100.0;
    return status;
}