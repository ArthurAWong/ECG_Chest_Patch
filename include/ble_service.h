/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HRS_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HRS_H_

/**
 * @brief Heart Rate Service (HRS)
 * @defgroup bt_hrs Heart Rate Service (HRS)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Notify ecg measurement.
 *
 * This will send a GATT notification to all current subscribers.
 *
 *  @param ecg_val The ecg measurement as read from device.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int bt_ecg_notify(int32_t val);
int bt_xaccel_notify(int16_t val);
int bt_yaccel_notify(int16_t val);
int bt_zaccel_notify(int16_t val);
int bt_gyrox_notify(int16_t val);
int bt_gyroy_notify(int16_t val);
int bt_gyroz_notify(int16_t val);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HRS_H_ */
