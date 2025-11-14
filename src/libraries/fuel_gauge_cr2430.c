#include "fuel_gauge_cr2430.h"

volatile double state_of_charge = 0;

static const struct battery_model_primary battery_models[] = {
	[BATTERY_TYPE_ALKALINE_AA] = {
		#include <battery_models/primary_cell/AA_Alkaline.inc>
	},
	[BATTERY_TYPE_ALKALINE_AAA] = {
		#include <battery_models/primary_cell/AAA_Alkaline.inc>
	},
	[BATTERY_TYPE_ALKALINE_2SAA] = {
		#include <battery_models/primary_cell/2SAA_Alkaline.inc>
	},
	[BATTERY_TYPE_ALKALINE_2SAAA] = {
		#include <battery_models/primary_cell/2SAAA_Alkaline.inc>
	},
	[BATTERY_TYPE_ALKALINE_LR44] = {
		#include <battery_models/primary_cell/LR44.inc>
	},
	[BATTERY_TYPE_LITHIUM_CR2032] = {
		#include <battery_models/primary_cell/CR2032.inc>
	},
};


static const float battery_current[] = {
	[BATTERY_TYPE_ALKALINE_AA] = 5e-3f,
	[BATTERY_TYPE_ALKALINE_AAA] = 5e-3f,
	[BATTERY_TYPE_ALKALINE_2SAA] = 5e-3f,
	[BATTERY_TYPE_ALKALINE_2SAAA] = 5e-3f,
	[BATTERY_TYPE_ALKALINE_LR44] = 1.5e-3f,
	[BATTERY_TYPE_LITHIUM_CR2032] = 1.5e-3f,
};

volatile enum battery_type selected_battery_model;

volatile int64_t ref_time;

int fuel_gauge_init(enum battery_type battery){
    float tbat;
    float t_chip;
    float vbat;
    int32_t vbat_mv;

    struct nrf_fuel_gauge_init_parameters parameters = {
		.model_primary = &battery_models[battery],
		.i0 = 0.0f,
		.opt_params = NULL,
	};
	int ret;

    ret = measure_bat_mv(&vbat_mv);
    ret = get_temperature(&t_chip);

    vbat = ((float)vbat_mv)/1000.0;
    tbat = t_chip - 5;
    parameters.v0 = vbat;
    parameters.t0 = tbat;

    ret = nrf_fuel_gauge_init(&parameters, NULL);
    if(ret<0){
        return ret;
    }

    ref_time = k_uptime_get();
    selected_battery_model = battery;

    return 0;
}


int fuel_gauge_update(double *soc){
    int err;
    float tbat;
    float t_chip;
    float vbat;
    int32_t vbat_mv;
    float delta;

    err = measure_bat_mv(&vbat_mv);
    err = get_temperature(&t_chip);

    vbat = ((float)vbat_mv)/1000.0;
    tbat = t_chip - 5;
    delta = (float)k_uptime_delta(&ref_time) / 1000.f;
    
    *soc = nrf_fuel_gauge_process(
		            vbat,
                    battery_current[selected_battery_model],
                    tbat,
                    delta,
                    NULL);
    
    return 0;
}