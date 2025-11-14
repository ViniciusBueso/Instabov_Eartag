#ifndef BLE_ADV_H
#define BLE_ADV_H

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>


//Declare the advertising packet
extern struct bt_data ad[];

//Declare ad size
extern size_t ad_size;

//Declare extended advertising instance
extern struct bt_le_ext_adv *adv;

//Declare variable to configure adv parameters
extern struct bt_le_adv_param *adv_param;

typedef union{
	uint32_t value;
	struct{
		uint32_t bat : 8;
		uint32_t step : 24;
	} __attribute__((packed)) field;
} custom_data_type;

//Declara estrutura que armazena dados customizados de advertising
typedef struct adv_mfg_data {
	uint16_t company_code;	    /* Company Identifier Code. */
	custom_data_type adv_data;
} __attribute__((packed)) adv_mfg_data_type;


extern volatile adv_mfg_data_type adv_mfg_data;

int configure_ext_adv(void);

//update adv custom data function
int update_adv_custom_data(uint32_t step_counter, uint8_t bat_lvl);

#endif