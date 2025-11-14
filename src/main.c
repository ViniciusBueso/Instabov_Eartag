#include <zephyr/kernel.h>
#include "ble/ble_adv.h"
#include "peripherals/my_gpio.h"
#include "peripherals/my_adc.h"
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/addr.h>
#include <hal/nrf_ficr.h>
#include <zephyr/bluetooth/hci_vs.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/sys/byteorder.h>
#include "peripherals/my_temp.h"

int err;
volatile uint32_t test_steps=0;
volatile uint8_t test_bat=0;
volatile uint32_t dev_id0, dev_id1, addr0, addr1;
volatile uint8_t addr_type;

typedef union{
    struct{
        uint32_t addr0;
        uint32_t addr1;
    }__attribute__((packed)) fields32;
    char byte_val[8];
}__attribute__((packed)) addr_ble_type;

void set_BLE_rnd_addr_from_FICR(bt_addr_le_t *addr);
static void set_tx_power(uint8_t handle_type, uint16_t handle, int8_t tx_pwr_lvl);

bt_addr_le_t addr; //variável que guarda o endereço ble

int main(void)
{
    //Read unique device ID
    dev_id0 = nrf_ficr_deviceid_get(NRF_FICR, 0);
    dev_id1 = nrf_ficr_deviceid_get(NRF_FICR, 1);
    
    //Set BLE address from FICR register
    set_BLE_rnd_addr_from_FICR(&addr);

    //Initialize temperature sensor
    err = init_temp_peripheral();
    if(err!=NRFX_SUCCESS){
        printk("temp error\r\n");
    }
    
    //Initialize ADC
    err  = adc_initialize();   
    printk("err0=%d\r\n", err);   
    
    //Register the address as Identity Address
	err = bt_id_create(&addr, NULL);
    printk("err1=%d\r\n", err);

    /* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
    printk("err2=%d\r\n", err);

    err = configure_ext_adv();
    printk("err3=%d\r\n", err);

    err = bt_le_ext_adv_start(adv, BT_LE_EXT_ADV_START_DEFAULT);
    printk("err4=%d\r\n", err);

    
    set_tx_power(BT_HCI_VS_LL_HANDLE_TYPE_ADV, 0, 7);

    while(1){
        test_steps++;
        test_bat=99;
        err = update_adv_custom_data(test_steps, test_bat);
        err = measure_bat_mv(&bat_meas_mv);
        err = get_temperature(&chip_temp);

        printk("BAT meas = %dmV ||", bat_meas_mv);
        if(chip_temp<300.0){
            printk("Temperature = %.2fC         \r", chip_temp);
        }
        k_sleep(K_MSEC(1000));
    }
    return 0;
}


void set_BLE_rnd_addr_from_FICR(bt_addr_le_t *addr){
    addr_ble_type addr_ble;
    addr_ble.fields32.addr0 = nrf_ficr_deviceaddr_get(NRF_FICR, 0);
    addr_ble.fields32.addr1 = nrf_ficr_deviceaddr_get(NRF_FICR, 1);
    addr_ble.byte_val[5] = addr_ble.byte_val[5] | 0xC0;
    for(uint8_t idx = 0; idx<6; idx++){
        addr->a.val[idx] = addr_ble.byte_val[idx];
    }
    addr->type = BT_ADDR_LE_RANDOM;

}


static void set_tx_power(uint8_t handle_type, uint16_t handle, int8_t tx_pwr_lvl)
{
	struct bt_hci_cp_vs_write_tx_power_level *cp;
	struct bt_hci_rp_vs_write_tx_power_level *rp;
	struct net_buf *buf, *rsp = NULL;
	int err;

	buf = bt_hci_cmd_create(BT_HCI_OP_VS_WRITE_TX_POWER_LEVEL,
				sizeof(*cp));
	if (!buf) {
		printk("Unable to allocate command buffer\n");
		return;
	}

	cp = net_buf_add(buf, sizeof(*cp));
	cp->handle = sys_cpu_to_le16(handle);
	cp->handle_type = handle_type;
	cp->tx_power_level = tx_pwr_lvl;

	err = bt_hci_cmd_send_sync(BT_HCI_OP_VS_WRITE_TX_POWER_LEVEL,
				   buf, &rsp);
	if (err) {
		printk("Set Tx power err: %d\n", err);
		return;
	}

	rp = (void *)rsp->data;
	printk("Actual Tx Power: %d\n", rp->selected_tx_power);

	net_buf_unref(rsp);
}