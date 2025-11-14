#include "ble_adv.h"

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define ADV_INTERVAL(x) ((uint16_t)(x/0.625)) 

#define COMPANY_ID_CODE 0x0059 //Nordic company ID

volatile adv_mfg_data_type adv_mfg_data = {COMPANY_ID_CODE, 0x00000000};

//Declare the advertising packet
struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),              //Bluetooth Classico não suportado e procedimento de descoberta = General Discovery
    BT_DATA(BT_DATA_MANUFACTURER_DATA,(unsigned char *)&adv_mfg_data, sizeof(adv_mfg_data)), //adiciona a estrutura AD com o dado customizado
};

//Declare ad size   
size_t ad_size = ARRAY_SIZE(ad);


//Declare extended advertising instance
struct bt_le_ext_adv *adv;


//Declare variable to configure adv parameters
struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
                                        (BT_LE_ADV_OPT_EXT_ADV|
                                            BT_LE_ADV_OPT_CODED|
                                            BT_LE_ADV_OPT_REQUIRE_S8_CODING|
                                            BT_LE_ADV_OPT_USE_IDENTITY
                                        ), //non-connectable, non-scannable, extended advertising using Coded Phy with S=8
	                                    ADV_INTERVAL(1000),            //intervalo mínimo de advertising em ms
	                                    ADV_INTERVAL(2000),            //intervalo maximo de advertising em ms
	                                    NULL                          //Endereço da central caso estejamos utilizando Directed Advertising
                                    );


int configure_ext_adv(void){
    int err;

    /* Create a connectable advertising set */
	err = bt_le_ext_adv_create(adv_param, NULL, &adv);

    if(err!=0){
        return err;
    }

    /* Configure ad data*/
    err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);

    return err;
}

//update adv custom data function
int update_adv_custom_data(uint32_t step_counter, uint8_t bat_lvl){
    adv_mfg_data.adv_data.field.bat = bat_lvl;
    adv_mfg_data.adv_data.field.step = step_counter;
    return bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
}