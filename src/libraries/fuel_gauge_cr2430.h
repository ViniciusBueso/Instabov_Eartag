#ifndef FUEL_GAUGE_CR2430_H
#define FUEL_GAUGE_CR2430_H
#include <nrf_fuel_gauge.h>
#include "../peripherals/my_adc.h"
#include "../peripherals/my_temp.h"


enum battery_type {
	/* Cylindrical non-rechargeable Alkaline AA */
	BATTERY_TYPE_ALKALINE_AA,
	/* Cylindrical non-rechargeable Alkaline AAA */
	BATTERY_TYPE_ALKALINE_AAA,
	/* Cylindrical non-rechargeable Alkaline 2SAA (2 x AA in series) */
	BATTERY_TYPE_ALKALINE_2SAA,
	/* Cylindrical non-rechargeable Alkaline 2SAAA (2 x AAA in series) */
	BATTERY_TYPE_ALKALINE_2SAAA,
	/* Alkaline coin cell LR44 */
	BATTERY_TYPE_ALKALINE_LR44,
	/* Lithium-manganese dioxide coin cell CR2032 */
	BATTERY_TYPE_LITHIUM_CR2032,
};


extern volatile enum battery_type selected_battery_model;

extern volatile int64_t ref_time;

extern volatile double state_of_charge;


int fuel_gauge_init(enum battery_type battery);
int fuel_gauge_update(double *soc);



#endif