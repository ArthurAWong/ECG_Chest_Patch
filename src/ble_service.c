/** @file
 *  @brief HRS Service sample
 */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/zephyr.h>
#include <zephyr/init.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#define LOG_LEVEL CONFIG_BT_HRS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ecg);


#define GATT_PERM_READ_MASK     (BT_GATT_PERM_READ | \
				 BT_GATT_PERM_READ_ENCRYPT | \
				 BT_GATT_PERM_READ_AUTHEN)
#define GATT_PERM_WRITE_MASK    (BT_GATT_PERM_WRITE | \
				 BT_GATT_PERM_WRITE_ENCRYPT | \
				 BT_GATT_PERM_WRITE_AUTHEN)

#ifndef CONFIG_BT_HRS_DEFAULT_PERM_RW_AUTHEN
#define CONFIG_BT_HRS_DEFAULT_PERM_RW_AUTHEN 0
#endif
#ifndef CONFIG_BT_HRS_DEFAULT_PERM_RW_ENCRYPT
#define CONFIG_BT_HRS_DEFAULT_PERM_RW_ENCRYPT 0
#endif
#ifndef CONFIG_BT_HRS_DEFAULT_PERM_RW
#define CONFIG_BT_HRS_DEFAULT_PERM_RW 0
#endif

#define HRS_GATT_PERM_DEFAULT (						\
	CONFIG_BT_HRS_DEFAULT_PERM_RW_AUTHEN ?				\
	(BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN) :	\
	CONFIG_BT_HRS_DEFAULT_PERM_RW_ENCRYPT ?				\
	(BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT) :	\
	(BT_GATT_PERM_READ | BT_GATT_PERM_WRITE))			\

// Custom service UUID
#define BT_UUID_ECG_SERVICE_VAL BT_UUID_128_ENCODE(0xed6c43b9, 0xb481, 0x4735, 0xad3a, 0x182d050c3688) //ed6c43b9-b481-4735-ad3a-182d050c3688
#define BT_UUID_ECG_SERVICE BT_UUID_DECLARE_128(BT_UUID_ECG_SERVICE_VAL)

// Custom ECG characteristic UUID
#define BT_ECG_CHRC_VAL BT_UUID_128_ENCODE(0x60a19d0d, 0x7e6c, 0x4d6a, 0xac49, 0x9b3c29a0ca94) //60a19d0d-7e6c-4d6a-ac49-9b3c29a0ca94
#define BT_ECG_CHRC BT_UUID_DECLARE_128(BT_ECG_CHRC_VAL)

// Custom accelerometer service UUID
#define BT_UUID_ACCELEROMETER_SERVICE_VAL1 BT_UUID_128_ENCODE(0xb35a0ebb, 0x4f41, 0x41b0, 0x8d70, 0x96b4dc15d9e7) //b35a0ebb-4f41-41b0-8d70-96b4dc15d9e7
#define BT_UUID_ACCELEROMETER_SERVICE1 BT_UUID_DECLARE_128(BT_UUID_ACCELEROMETER_SERVICE_VAL1)

// Custom accelerometer x characteristic UUID
#define BT_XACCEL_CHRC_VAL BT_UUID_128_ENCODE(0x3b8cdf21, 0x24a2, 0x468c, 0x9236, 0x20f50b770bf5) //3b8cdf21-24a2-468c-9236-20f50b770bf5
#define BT_XACCEL_CHRC BT_UUID_DECLARE_128(BT_XACCEL_CHRC_VAL)

// Custom accelerometer y characteristic UUID
#define BT_YACCEL_CHRC_VAL BT_UUID_128_ENCODE(0x1094b4ef, 0x76bd, 0x4341, 0xaa96, 0x83f99198db63) //1094b4ef-76bd-4341-aa96-83f99198db63
#define BT_YACCEL_CHRC BT_UUID_DECLARE_128(BT_YACCEL_CHRC_VAL)

// Custom accelerometer z characteristic UUID
#define BT_ZACCEL_CHRC_VAL BT_UUID_128_ENCODE(0xf5f4e4fe, 0x1ab0, 0x4aea, 0x9334, 0xb19fc27d6e7c) //f5f4e4fe-1ab0-4aea-9334-b19fc27d6e7c
#define BT_ZACCEL_CHRC BT_UUID_DECLARE_128(BT_ZACCEL_CHRC_VAL)

// Custom accelerometer gyro characteristic UUID
#define BT_GYRO_CHRC_VAL BT_UUID_128_ENCODE(0xff3675ff, 0x5479, 0x4e1c, 0x9d87, 0xa0ac6e2dadbf) //ff3675ff-5479-4e1c-9d87-a0ac6e2dadbf
#define BT_GYRO_CHRC BT_UUID_DECLARE_128(BT_GYRO_CHRC_VAL)


static uint16_t ecg_reading;
static uint32_t accelx_reading;
static uint32_t accely_reading;
static uint32_t accelz_reading;
static uint32_t gyro_reading;

static void ecg_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("notifications %s", notif_enabled ? "enabled" : "disabled");
}

static void xaccel_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("notifications %s", notif_enabled ? "enabled" : "disabled");
}

static void yaccel_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("notifications %s", notif_enabled ? "enabled" : "disabled");
}

static void zaccel_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("notifications %s", notif_enabled ? "enabled" : "disabled");
}

static void gyro_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("notifications %s", notif_enabled ? "enabled" : "disabled");
}


/* ECG Service Declaration */
BT_GATT_SERVICE_DEFINE(ecg_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_ECG_SERVICE),
	BT_GATT_CHARACTERISTIC(BT_ECG_CHRC, BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(ecg_ccc_cfg_changed,
		    HRS_GATT_PERM_DEFAULT),
);

/* Accelerometer Service Declaration */
BT_GATT_SERVICE_DEFINE(accelerometer_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_ACCELEROMETER_SERVICE1),
	BT_GATT_CHARACTERISTIC(BT_XACCEL_CHRC, BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(xaccel_ccc_cfg_changed,
		    HRS_GATT_PERM_DEFAULT),
	BT_GATT_CHARACTERISTIC(BT_YACCEL_CHRC, BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(yaccel_ccc_cfg_changed,
		    HRS_GATT_PERM_DEFAULT),
	BT_GATT_CHARACTERISTIC(BT_ZACCEL_CHRC, BT_GATT_CHRC_NOTIFY,
				BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(zaccel_ccc_cfg_changed,
		    HRS_GATT_PERM_DEFAULT),
	BT_GATT_CHARACTERISTIC(BT_GYRO_CHRC, BT_GATT_CHRC_NOTIFY,
				BT_GATT_PERM_NONE, NULL, NULL, NULL),
	BT_GATT_CCC(gyro_ccc_cfg_changed,
		    HRS_GATT_PERM_DEFAULT),
	
);

static int chestpatch_val_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	ecg_reading = 0x00;
	accelx_reading = 0x00;
	accely_reading = 0x00;
	accelz_reading = 0x00;
	gyro_reading = 0x00;
	return 0;
}

int bt_ecg_notify(uint16_t val)
{
	int rc;
	static uint16_t ecg;

	ecg = val;

	rc = bt_gatt_notify(NULL, &ecg_svc.attrs[1], &ecg, sizeof(ecg));

	return rc == -ENOTCONN ? 0 : rc;
}

int bt_xaccel_notify(uint16_t val)
{
	int rc;
	static uint16_t xaccel;

	xaccel = val;

	rc = bt_gatt_notify(NULL, &accelerometer_svc.attrs[1], &xaccel, sizeof(xaccel));

	return rc == -ENOTCONN ? 0 : rc;
}

int bt_yaccel_notify(uint16_t val)
{
	int rc;
	static uint16_t yaccel;

	yaccel = val;

	rc = bt_gatt_notify(NULL, &accelerometer_svc.attrs[2], &yaccel, sizeof(yaccel));

	return rc == -ENOTCONN ? 0 : rc;
}

int bt_zaccel_notify(uint16_t val)
{
	int rc;
	static uint16_t zaccel;

	zaccel = val;

	rc = bt_gatt_notify(NULL, &accelerometer_svc.attrs[3], &zaccel, sizeof(zaccel));

	return rc == -ENOTCONN ? 0 : rc;
}

int bt_gyro_notify(uint16_t val)
{
	int rc;
	static uint16_t gyro;

	gyro = val;

	rc = bt_gatt_notify(NULL, &accelerometer_svc.attrs[4], &gyro, sizeof(gyro));

	return rc == -ENOTCONN ? 0 : rc;
}

SYS_INIT(chestpatch_val_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
