
// /*
//  * Copyright (c) 2012-2014 Wind River Systems, Inc.
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */
// #include "max30003.h"

// void main(void)
// {	
// 	//setup_interrupt();
	// max_reset_sw();
	// max_readinfo();
	// max_readstatus();
	// max_enable_ecg();
// }

/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ble_service.h"
#include "max30003.h"
#include "lsm6dsm.h"

#include <zephyr/drivers/gpio.h>

#define ECG_CHARACTERISTIC_STUFFING 30
#define ACCEL_CHARACTERISTIC_STUFFING 15 /*ENSURE IS MULTIPLE OF 3*/
#define GYRO_CHARACTERISTIC_STUFFING 15 /*ENSURE IS MULTIPLE OF 3*/

struct gpio_dt_spec led = 
	{
		.port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
		.pin = 20,
		.dt_flags = GPIO_ACTIVE_LOW
	};

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		      BT_UUID_16_ENCODE(BT_UUID_DIS_VAL)),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
	} else {
		printk("Connected\n");
		gpio_pin_set_dt(&led, 1);
		bt_conn_le_phy_update(conn, BT_CONN_LE_PHY_PARAM_2M);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
	gpio_pin_set_dt(&led, 0);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_ready(void)
{
	int err;

	printk("Bluetooth initialized\n");

	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.cancel = auth_cancel,
};

static void ecg_notify(int32_t *ptr, uint8_t len)
{
	// Actual way to do it, for now commented out
	// TODO: uncomment the following code
	// static uint8_t ecg_val[3] = {0x00, 0x00, 0x00};
	// int err = max30003_read(ECG_FIFO, &ecg_val, sizeof(ecg_val));
	// uint32_t ecg_data = ((ecg_val[0] << 16) | (ecg_val[1] << 8) | (ecg_val[2] << 0)) >> 6;
	// uint16_t eight_bit_ECG_data = (uint16_t) ecg_data;

	// Following code is just a test for Vince's debugging
	// static int16_t eight_bit_ECG_data = 4;
	// eight_bit_ECG_data += 1;
	// if (eight_bit_ECG_data == 100)
	// {
	// 	eight_bit_ECG_data = 0;
	// }

	bt_ecg_notify(ptr, len);
	//bt_ecg_notify(eight_bit_ECG_data);
}

static void accel_notify(int16_t *ptr, uint8_t len)
{

	// Following code is just a test for Vince's debugging
	// TODO: replace with actual read accelerometer code
	// static int16_t xaccel_data = 0xfade;
	// xaccel_data += 1;
	// if (xaccel_data == 100)
	// {
	// 	xaccel_data = 0;
	// }

	bt_accel_notify(ptr, len);
}

// static void yaccel_notify(void)
// {

// 	// Following code is just a test for Vince's debugging
// 	// TODO: replace with actual read accelerometer code
// 	static int16_t yaccel_data = 1;
// 	yaccel_data += 1;
// 	if(yaccel_data == 100)
// 	{
// 		yaccel_data = 0;
// 	}

// 	bt_yaccel_notify(accel_xyz[1]);
// }

// static void zaccel_notify(void)
// {

// 	// Following code is just a test for Vince's debugging
// 	// TODO: replace with actual read accelerometer code
// 	static int16_t zaccel_data = 2;
// 	zaccel_data += 2;
// 	if(zaccel_data == 100)
// 	{
// 		zaccel_data = 0;
// 	}

// 	bt_zaccel_notify(accel_xyz[2]);
// }

static void gyro_notify(int16_t *ptr, uint8_t len)
{

	// // Following code is just a test for Vince's debugging
	// // TODO: replace with actual read accelerometer code
	// static uint32_t gyro_data = 2;
	// gyro_data += 1;
	// if(gyro_data == 100)
	// {
	// 	gyro_data = 0;
	// }
	// static int16_t zaccel_data = 2;
	// zaccel_data += 2;
	// if (zaccel_data == 100)
	// {
	// 	zaccel_data = 0;
	// }

	bt_gyro_notify(ptr, len);
}

// static void gyroy_notify(void)
// {

// 	// // Following code is just a test for Vince's debugging
// 	// // TODO: replace with actual read accelerometer code
// 	// static uint32_t gyro_data = 3;
// 	// gyro_data += 1;
// 	// if(gyro_data == 100)
// 	// {
// 	// 	gyro_data = 0;
// 	// }
// 	static int16_t zaccel_data = 2;
// 	zaccel_data += 2;
// 	if (zaccel_data == 100)
// 	{
// 		zaccel_data = 0;
// 	}

// 	bt_gyroy_notify(gyro_xyz[1]);
// }

// static void gyroz_notify(void)
// {

// 	// // Following code is just a test for Vince's debugging
// 	// // TODO: replace with actual read accelerometer code
// 	// static uint32_t gyro_data = 4;
// 	// int16_t gyro_xyz[3] = {};
// 	// lsm6dsm_read_gyro(gyro_xyz);
// 	static int16_t zaccel_data = 2;
// 	zaccel_data += 2;
// 	if (zaccel_data == 100)
// 	{
// 		zaccel_data = 0;
// 	}

// 	bt_gyroz_notify(gyro_xyz[2]);
// }

void main(void)
{
	int err;
	int ret;

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	lsm6dsm_reset_sw();
	max_reset_sw();
	// max_readstatus();

	lsm6dsm_enable_accel();
	lsm6dsm_enable_gyro();
	max_enable_ecg();

	err = bt_enable(NULL);
	if (err)
	{
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_ready();

	bt_conn_auth_cb_register(&auth_cb_display);

	uint8_t lsm6dsm_status;

	int i = 0;
	int j = 0;
	int k = 0;

	int16_t accel_xyz[3] = {};
	int16_t gyro_xyz[3] = {};
	int32_t ecg_num;

	int32_t ecg_arr[ECG_CHARACTERISTIC_STUFFING];
	int16_t accel_arr[ACCEL_CHARACTERISTIC_STUFFING];
	int16_t gyro_arr[GYRO_CHARACTERISTIC_STUFFING];

	/* Implement notification. At the moment there is no suitable way
	 * of starting delayed work so we do it here
	 */
	while (1) {
		// xaccel_notify();
		// yaccel_notify();
		// zaccel_notify();
		// gyrox_notify();
		// gyroy_notify();
		// gyroz_notify();
		// ecg_notify();
		// while(1) {
		// 	if (lsm6dsm_read_who_am_i())
		// 	{
		// 	k_msleep(250);
		// 	ret = gpio_pin_toggle_dt(&led);
		// 	}
		// 	max_readinfo();
		// 	if ()
		// 	{
		// 		lsm6dsm_good = 1;
		// 		yaccel_notify();
		// 	} else {
		// 		lsm6dsm_good = 0;
		// 		yaccel_notify();
		// 	}
		// }

		ret = lsm6dsm_read_status(&lsm6dsm_status);
		if (lsm6dsm_status & (1 << 0))
		{
			ret = lsm6dsm_read_accel(accel_xyz);
			if (!ret)
			{
				accel_arr[i] = accel_xyz[0];
				i += 1;
				accel_arr[i+1] = accel_xyz[1];
				i += 1;
				accel_arr[i+2] = accel_xyz[2];
				i += 1;
				if (i >= ACCEL_CHARACTERISTIC_STUFFING)
				{
					accel_notify(accel_arr, ACCEL_CHARACTERISTIC_STUFFING);
					i = 0;
				}
				// printf("Accel x: % 3.3f, y: % 3.3f, z: % 3.3f\n", ((float)accel_xyz[0]) * 0.061 / 1000 * 9.81, ((float)accel_xyz[1]) * 0.061 / 1000 * 9.81, ((float)accel_xyz[2]) * 0.061 / 1000 * 9.81);
			}
		}

		if (lsm6dsm_status & (1 << 1))
		{
			ret = lsm6dsm_read_gyro(gyro_xyz);
			if (!ret)
			{
				gyro_arr[j] = gyro_xyz[0];
				j += 1;
				gyro_arr[j] = gyro_xyz[1];
				j += 1;
				gyro_arr[j] = gyro_xyz[2];
				j += 1;
				if (j >= GYRO_CHARACTERISTIC_STUFFING)
				{
					gyro_notify(gyro_arr, GYRO_CHARACTERISTIC_STUFFING);
					j=0;
				}
				// gyrox_notify();
				// gyroy_notify();
				// gyroz_notify();
				// printf("Gyro x: % 3.3f, y: % 3.3f, z: % 3.3f\n", (gyro_xyz[0]) * 8.75 / 1000, (gyro_xyz[1]) * 8.75 / 1000, (gyro_xyz[2]) * 8.75 / 1000);
			}
		}
		ret = max_read_ecg(&ecg_num);
		if (!ret)
		{
			ecg_arr[k] = ecg_num;
			k++;
			// printk("%d\n", ecg_num);
			if (k >= ECG_CHARACTERISTIC_STUFFING)
			{
				ecg_notify(ecg_arr, ECG_CHARACTERISTIC_STUFFING);
				k = 0;
			}
		}
	}
}

