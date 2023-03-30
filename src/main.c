
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
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
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

static void ecg_notify(void)
{
	// Actual way to do it, for now commented out
	// TODO: uncomment the following code
	// static uint8_t ecg_val[3] = {0x00, 0x00, 0x00};
	// int err = max30003_read(ECG_FIFO, &ecg_val, sizeof(ecg_val));
	// uint32_t ecg_data = ((ecg_val[0] << 16) | (ecg_val[1] << 8) | (ecg_val[2] << 0)) >> 6;
	// uint16_t eight_bit_ECG_data = (uint16_t) ecg_data;

	// Following code is just a test for Vince's debugging
	static uint16_t eight_bit_ECG_data = 4;
	eight_bit_ECG_data += 1;
	if(eight_bit_ECG_data == 100)
	{
		eight_bit_ECG_data = 0;
	}

	bt_ecg_notify(eight_bit_ECG_data);
}

static void xaccel_notify(void)
{

	// Following code is just a test for Vince's debugging
	// TODO: replace with actual read accelerometer code
	static uint32_t xaccel_data = 0;
	xaccel_data += 1;
	if(xaccel_data == 100)
	{
		xaccel_data = 0;
	}

	bt_xaccel_notify(xaccel_data);
}

static void yaccel_notify(void)
{

	// Following code is just a test for Vince's debugging
	// TODO: replace with actual read accelerometer code
	static uint32_t yaccel_data = 1;
	yaccel_data += 1;
	if(yaccel_data == 100)
	{
		yaccel_data = 0;
	}

	bt_yaccel_notify(yaccel_data);
}

static void zaccel_notify(void)
{

	// Following code is just a test for Vince's debugging
	// TODO: replace with actual read accelerometer code
	static uint32_t zaccel_data = 2;
	zaccel_data += 2;
	if(zaccel_data == 100)
	{
		zaccel_data = 0;
	}

	bt_zaccel_notify(zaccel_data);
}

static void gyro_notify(void)
{

	// Following code is just a test for Vince's debugging
	// TODO: replace with actual read accelerometer code
	static uint32_t gyro_data = 3;
	gyro_data += 1;
	if(gyro_data == 100)
	{
		gyro_data = 0;
	}

	bt_gyro_notify(gyro_data);
}

void main(void)
{
	int err;

	max_reset_sw();
	max_readinfo();
	max_readstatus();
	max_enable_ecg();

	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_ready();

	bt_conn_auth_cb_register(&auth_cb_display);

	/* Implement notification. At the moment there is no suitable way
	 * of starting delayed work so we do it here
	 */
	while (1) {
		k_sleep(K_SECONDS(1));

		/* Heartrate measurements simulation */
		ecg_notify();
		xaccel_notify();
		yaccel_notify();
		zaccel_notify();
		gyro_notify();
		/* Battery level simulation */
		// bas_notify();
	}
}

