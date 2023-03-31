#include <stdio.h>
#include <zephyr/zephyr.h>

#ifndef LSM6DSM
#define LSM6DSM

#define BUF_SIZE 2

/*********** REGISTERS ***********/

/* Setup registers */
#define LSM6DSM_FUNC_CFG_ACCESS          (uint8_t)0x01U  /**< Enable embedded functions register (r/w) **/
#define LSM6DSM_SENSOR_SYNC_TIME_FRAME   (uint8_t)0x04U  /**< Sensor synchronization time frame register (r/w) **/
#define LSM6DSM_SENSOR_SYNC_RES_RATIO    (uint8_t)0x05U  /**< Sensor synchronization resolution ratio (r/w) **/

#define LSM6DSM_FIFO_CTRL1               (uint8_t)0x06U  /**< FIFO control register 1 (r/w) **/
#define LSM6DSM_FIFO_CTRL2               (uint8_t)0x07U  /**< FIFO control register 2 (r/w) **/
#define LSM6DSM_FIFO_CTRL3               (uint8_t)0x08U  /**< FIFO control register 3 (r/w) **/
#define LSM6DSM_FIFO_CTRL4               (uint8_t)0x09U  /**< FIFO control register 4 (r/w) **/
#define LSM6DSM_FIFO_CTRL5               (uint8_t)0x0AU  /**< FIFO control register 5 (r/w) **/

#define LSM6DSM_DRDY_PULSE_CFG_G         (uint8_t)0x0BU  /**< DataReady configuration register (r/w)**/
#define LSM6DSM_INT1_CTRL                (uint8_t)0x0DU  /**< INT1 pad control register (r/w) **/
#define LSM6DSM_INT2_CTRL                (uint8_t)0x0EU  /**< INT2 pad control register (r/w) **/

#define LSM6DSM_WHO_AM_I                 (uint8_t)0x0FU  /**< Who_AM_I register (r), value fixed at 6Ah **/

#define LSM6DSM_CTRL1_XL                 (uint8_t)0x10U  /**< Linear acceleration sensor control register 1 (r/w) **/
#define LSM6DSM_CTRL2_G                  (uint8_t)0x11U  /**< Angular rate sensor control register 2 (r/w) **/
#define LSM6DSM_CTRL3_C                  (uint8_t)0x12U  /**< Control register 3 (r/w) **/
#define LSM6DSM_CTRL4_C                  (uint8_t)0x13U  /**< Control register 4 (r/w) **/
#define LSM6DSM_CTRL5_C                  (uint8_t)0x14U  /**< Control register 5 (r/w) **/
#define LSM6DSM_CTRL6_C                  (uint8_t)0x15U  /**< Angular rate sensor control register 6 (r/w) **/
#define LSM6DSM_CTRL7_G                  (uint8_t)0x16U  /**< Angular rate sensor control register 7 (r/w) **/
#define LSM6DSM_CTRL8_XL                 (uint8_t)0x17U  /**< Linear acceleration sensor control register 8 (r/w) **/
#define LSM6DSM_CTRL9_XL                 (uint8_t)0x18U  /**< Linear acceleration sensor control register 9 (r/w) **/
#define LSM6DSM_CTRL10_C                 (uint8_t)0x19U  /**< Control register 10 (r/w) **/

#define LSM6DSM_MASTER_CONFIG            (uint8_t)0x1AU  /**< Master configuration register (r/w) **/
#define LSM6DSM_WAKE_UP_SRC              (uint8_t)0x1BU  /**< Wake up interrupt source register (r) **/
#define LSM6DSM_TAP_SRC                  (uint8_t)0x1CU  /**< Tap source register (r) **/
#define LSM6DSM_D6D_SRC                  (uint8_t)0x1DU  /**< Portrait, landscape, face-up and face-down source register (r) **/
#define LSM6DSM_STATUS_REG               (uint8_t)0x1EU  /**< Status register (r) **/


#define LSM6DSM_OUT_TEMP_L               (uint8_t)0x20U  /**< Temperature data output register Bit 7..0 (r) **/
#define LSM6DSM_OUT_TEMP_H               (uint8_t)0x21U  /**< Temperature data output register Bit 15..8 (r) **/


#define LSM6DSM_OUTX_L_G                 (uint8_t)0x22U  /**< Angular rate pitch axis X Bit 7..0 (r)**/
#define LSM6DSM_OUTX_H_G                 (uint8_t)0x23U  /**< Angular rate pitch axis X Bit 15..8 (r)**/
#define LSM6DSM_OUTY_L_G                 (uint8_t)0x24U  /**< Angular rate pitch axis Y Bit 7..0 (r)**/
#define LSM6DSM_OUTY_H_G                 (uint8_t)0x25U  /**< Angular rate pitch axis Y Bit 15..8 (r)**/
#define LSM6DSM_OUTZ_L_G                 (uint8_t)0x26U  /**< Angular rate pitch axis Z Bit 7..0 (r)**/
#define LSM6DSM_OUTZ_H_G                 (uint8_t)0x27U  /**< Angular rate pitch axis Z Bit 15..8 (r)**/

#define LSM6DSM_OUTX_L_XL                (uint8_t)0x28U  /**< Linear acceleration axis X Bit 7..0 (r)**/
#define LSM6DSM_OUTX_H_XL                (uint8_t)0x29U  /**< Linear acceleration axis X Bit 15..8 (r)**/
#define LSM6DSM_OUTY_L_XL                (uint8_t)0x2AU  /**< Linear acceleration axis Y Bit 7..0 (r)**/
#define LSM6DSM_OUTY_H_XL                (uint8_t)0x2BU  /**< Linear acceleration axis Y Bit 15..8 (r)**/
#define LSM6DSM_OUTZ_L_XL                (uint8_t)0x2CU  /**< Linear acceleration axis Z Bit 7..0 (r)**/
#define LSM6DSM_OUTZ_H_XL                (uint8_t)0x2DU  /**< Linear acceleration axis Z Bit 15..8 (r)**/

/** refer to @ref lsm6dsm_embedded_registers for following sensorhub bytes functionality **/
#define LSM6DSM_SENSORHUB1_REG           (uint8_t)0x2EU  /**< byte 1 associated to external sensors **/
#define LSM6DSM_SENSORHUB2_REG           (uint8_t)0x2FU  /**< byte 2 associated to external sensors **/
#define LSM6DSM_SENSORHUB3_REG           (uint8_t)0x30U  /**< byte 3 associated to external sensors **/
#define LSM6DSM_SENSORHUB4_REG           (uint8_t)0x31U  /**< byte 4 associated to external sensors **/
#define LSM6DSM_SENSORHUB5_REG           (uint8_t)0x32U  /**< byte 5 associated to external sensors **/
#define LSM6DSM_SENSORHUB6_REG           (uint8_t)0x33U  /**< byte 6 associated to external sensors **/
#define LSM6DSM_SENSORHUB7_REG           (uint8_t)0x34U  /**< byte 7 associated to external sensors **/
#define LSM6DSM_SENSORHUB8_REG           (uint8_t)0x35U  /**< byte 8 associated to external sensors **/
#define LSM6DSM_SENSORHUB9_REG           (uint8_t)0x36U  /**< byte 9 associated to external sensors **/
#define LSM6DSM_SENSORHUB10_REG          (uint8_t)0x37U  /**< byte 10 associated to external sensors **/
#define LSM6DSM_SENSORHUB11_REG          (uint8_t)0x38U  /**< byte 11 associated to external sensors **/ 
#define LSM6DSM_SENSORHUB12_REG          (uint8_t)0x39U  /**< byte 12 associated to external sensors **/

/** For a proper reading of the following registers, 
 *  it is recommended to set the BDU bit in CTRL3_C (12h) to 1 **/
#define LSM6DSM_FIFO_STATUS1             (uint8_t)0x3AU  /**< FIFO status control 1 (r) **/
#define LSM6DSM_FIFO_STATUS2             (uint8_t)0x3BU  /**< FIFO status control 2 (r)**/
#define LSM6DSM_FIFO_STATUS3             (uint8_t)0x3CU  /**< FIFO status control 3 (r)**/
#define LSM6DSM_FIFO_STATUS4             (uint8_t)0x3DU  /**< FIFO status control 4 (r) **/

#define LSM6DSM_FIFO_DATA_OUT_L          (uint8_t)0x3EU  /**< FIFO data output - Bit 7..0 (r)**/
#define LSM6DSM_FIFO_DATA_OUT_H          (uint8_t)0x3FU  /**< FIFO data output Bit - 15..8 (r)**/

/** to reset the Timestamp timer, (uint8_t)0xAA has to be written in TIMESTAMP2_REG **/
#define LSM6DSM_TIMESTAMP0_REG           (uint8_t)0x40U  /**< Timestamp data output - Bit 7..0 (r) **/
#define LSM6DSM_TIMESTAMP1_REG           (uint8_t)0x41U  /**< Timestamp data output - Bit 15..8 (r) **/
#define LSM6DSM_TIMESTAMP2_REG           (uint8_t)0x42U  /**< Timestamp data output - Bit 23..16 (r/w) **/
#define LSM6DSM_STEP_TIMESTAMP_L         (uint8_t)0x49U  /**< Step counter timestamp - contains TIMESTAMP0 (r)**/
#define LSM6DSM_STEP_TIMESTAMP_H         (uint8_t)0x4AU  /**< Step counter timestamp - contains TIMESTAMP1 (r)**/
#define LSM6DSM_STEP_COUNTER_L           (uint8_t)0x4BU  /**< Step counter output register - Bit 7..0 (r) **/
#define LSM6DSM_STEP_COUNTER_H           (uint8_t)0x4CU  /**< Step counter output register - Bit 13..8 (r) **/

#define LSM6DSM_SENSORHUB13_REG          (uint8_t)0x4DU  /**< byte 13 associated to external sensors **/
#define LSM6DSM_SENSORHUB14_REG          (uint8_t)0x4EU  /**< byte 14 associated to external sensors **/
#define LSM6DSM_SENSORHUB15_REG          (uint8_t)0x4FU  /**< byte 15 associated to external sensors **/
#define LSM6DSM_SENSORHUB16_REG          (uint8_t)0x50U  /**< byte 16 associated to external sensors **/
#define LSM6DSM_SENSORHUB17_REG          (uint8_t)0x51U  /**< byte 17 associated to external sensors **/
#define LSM6DSM_SENSORHUB18_REG          (uint8_t)0x52U  /**< byte 18 associated to external sensors **/
  
#define LSM6DSM_FUNC_SRC1                (uint8_t)0x53U  /**< Significant motion, tilt, step detector,   **/ 
                                                    /** hard/soft-iron and sensor hub interrupt source (r) **/
#define LSM6DSM_FUNC_SRC2                (uint8_t)0x54U  /**< Wrist tilt and slave NAK interrupt source (r) **/

#define LSM6DSM_WRIST_TILT_IA            (uint8_t)0x55U  /**< Wrist tilt interrupt source (r) **/
#define LSM6DSM_TAP_CFG                  (uint8_t)0x58U  /**< interrupt, inactivity function , filtering (r/w) **/
#define LSM6DSM_TAP_THS_6D               (uint8_t)0x59U  /**< Portrait/landscape, tap function threshold (r/w) **/
#define LSM6DSM_INT_DUR2                 (uint8_t)0x5AU  /**< Tap recognition function setting (r/w) **/
#define LSM6DSM_WAKE_UP_THS              (uint8_t)0x5BU  /**< Single and double-tap function threshold (r/w) **/
#define LSM6DSM_WAKE_UP_DUR              (uint8_t)0x5CU  /**< Free-fall, wakeup, timestamp, sleep duration (r/w) **/
#define LSM6DSM_FREE_FALL                (uint8_t)0x5DU  /**< Free-fall function duration setting register (r/w) **/
#define LSM6DSM_MD1_CFG                  (uint8_t)0x5EU  /**< Functions routing on INT1 register (r/w) **/
#define LSM6DSM_MD2_CFG                  (uint8_t)0x5FU  /**< Functions routing on INT2 register (r/w) **/

#define LSM6DSM_MASTER_CMD_CODE          (uint8_t)0x60U  /**< Master command code used for stamping **/
#define LSM6DSM_SENS_SYNC_SPI_ERROR_CODE (uint8_t)0x61U  /**< Error code used for sensor synchronization **/

#define LSM6DSM_OUT_MAG_RAW_X_L          (uint8_t)0x66U  /**< External magnetometer X data - Bit 7..0 (r) **/
#define LSM6DSM_OUT_MAG_RAW_X_H          (uint8_t)0x67U  /**< External magnetometer X data - Bit 15..8 (r) **/
#define LSM6DSM_OUT_MAG_RAW_Y_L          (uint8_t)0x68U  /**< External magnetometer Y data - Bit 7..0 (r) **/
#define LSM6DSM_OUT_MAG_RAW_Y_H          (uint8_t)0x69U  /**< External magnetometer Y data - Bit 15..8 (r) **/
#define LSM6DSM_OUT_MAG_RAW_Z_L          (uint8_t)0x6AU  /**< External magnetometer Z data - Bit 7..0 (r) **/
#define LSM6DSM_OUT_MAG_RAW_Z_H          (uint8_t)0x6BU  /**< External magnetometer Z data - Bit 15..8 (r) **/

#define LSM6DSM_INT_OIS                  (uint8_t)0x6FU  /**< OIS interrupt config (r) (AUX SPI (r/w)) **/
#define LSM6DSM_CTRL1_OIS                (uint8_t)0x70U  /**< OIS configuration 1 (r) (AUX SPI (r/w)) **/
#define LSM6DSM_CTRL2_OIS                (uint8_t)0x71U  /**< OIS configuration 2 (r) (AUX SPI (r/w)) **/
#define LSM6DSM_CTRL3_OIS                (uint8_t)0x72U  /**< OIS configuration 3 (r) (AUX SPI (r/w)) **/

#define LSM6DSM_X_OFS_USR                (uint8_t)0x73U  /**< Accelerometer X user offset correction (r/w) **/
#define LSM6DSM_Y_OFS_USR                (uint8_t)0x74U  /**< Accelerometer Y user offset correction (r/w) **/
#define LSM6DSM_Z_OFS_USR                (uint8_t)0x75U  /**< Accelerometer Z user offset correction (r/w) **/
/*! @} */

/*! @defgroup lsm6dsm_embedded_registers Group: embedded registers   
*  lsm6dsm embedded registers
*  @{
*/
#define LSM6DSM_SLV0_ADD                 (uint8_t)0x02U /**< I2C slave address Sensor   1 (r/w) **/
#define LSM6DSM_SLV0_SUBADD              (uint8_t)0x03U /**< Address of register Sensor 1 (r/w) **/
#define LSM6DSM_SLAVE0_CONFIG            (uint8_t)0x04U /**< configuration Sensor 1, sensor hub (r/w) **/
#define LSM6DSM_SLV1_ADD                 (uint8_t)0x05U /**< I2C slave address Sensor   2 (r/w) **/
#define LSM6DSM_SLV1_SUBADD              (uint8_t)0x06U /**< Address of register Sensor 2 (r/w) **/
#define LSM6DSM_SLAVE1_CONFIG            (uint8_t)0x07U /**< configuration Sensor       2 (r/w) **/
#define LSM6DSM_SLV2_ADD                 (uint8_t)0x08U /**< I2C slave address Sensor   3 (r/w) **/
#define LSM6DSM_SLV2_SUBADD              (uint8_t)0x09U /**< Address of register Sensor 3 (r/w) **/
#define LSM6DSM_SLAVE2_CONFIG            (uint8_t)0x0AU /**< configuration Sensor       3 (r/w) **/
#define LSM6DSM_SLV3_ADD                 (uint8_t)0x0BU /**< I2C slave address Sensor   4 (r/w) **/
#define LSM6DSM_SLV3_SUBADD              (uint8_t)0x0CU /**< Address of register Sensor 4 (r/w) **/
#define LSM6DSM_SLAVE3_CONFIG            (uint8_t)0x0DU /**< configuration Sensor       4 (r/w) **/

#define LSM6DSM_DATAWR_SRC_MODE_SUB_SLV0 (uint8_t)0x0EU /**< Write in SLV0 register  (r/w) **/
#define LSM6DSM_CONFIG_PEDO_THS_MIN      (uint8_t)0x0FU /**< Pedometer configuration (r/w) **/

#define LSM6DSM_SM_THS                   (uint8_t)0x13U /**< Significant motion configuration (r/w) **/
#define LSM6DSM_PEDO_DEB_REG             (uint8_t)0x14U /**< Pedometer debounce configuration (r/w) **/
#define LSM6DSM_STEP_COUNT_DELTA         (uint8_t)0x15U /**< Time period step detection  (r/w) **/

#define LSM6DSM_MAG_SI_XX                (uint8_t)0x24U /**< Soft-iron matrix correction row,col 1,1 (r/w)  **/
#define LSM6DSM_MAG_SI_XY                (uint8_t)0x25U /**< Soft-iron matrix correction row,col 1,2 (r/w)  **/
#define LSM6DSM_MAG_SI_XZ                (uint8_t)0x26U /**< Soft-iron matrix correction row,col 1,3 (r/w)  **/
#define LSM6DSM_MAG_SI_YX                (uint8_t)0x27U /**< Soft-iron matrix correction row,col 2,1 (r/w)  **/
#define LSM6DSM_MAG_SI_YY                (uint8_t)0x28U /**< Soft-iron matrix correction row,col 2,2 (r/w)  **/
#define LSM6DSM_MAG_SI_YZ                (uint8_t)0x29U /**< Soft-iron matrix correction row,col 2,3 (r/w)  **/
#define LSM6DSM_MAG_SI_ZX                (uint8_t)0x2AU /**< Soft-iron matrix correction row,col 3,1 (r/w)  **/
#define LSM6DSM_MAG_SI_ZY                (uint8_t)0x2BU /**< Soft-iron matrix correction row,col 3,2 (r/w)  **/
#define LSM6DSM_MAG_SI_ZZ                (uint8_t)0x2CU /**< Soft-iron matrix correction row,col 3,3 (r/w)  **/

#define LSM6DSM_MAG_OFFX_L               (uint8_t)0x2DU /**< Offset X-axis hard-iron compensation Bit 7..0 (r/w)**/
#define LSM6DSM_MAG_OFFX_H               (uint8_t)0x2EU /**< Offset X-axis hard-iron compensation Bit 15..8 (r/w)**/
#define LSM6DSM_MAG_OFFY_L               (uint8_t)0x2FU /**< Offset Y-axis hard-iron compensation Bit 7..0 (r/w)**/
#define LSM6DSM_MAG_OFFY_H               (uint8_t)0x30U /**< Offset Y-axis hard-iron compensation Bit 15..8 (r/w)**/
#define LSM6DSM_MAG_OFFZ_L               (uint8_t)0x31U /**< Offset Z-axis hard-iron compensation Bit 7..0 (r/w)**/
#define LSM6DSM_MAG_OFFZ_H               (uint8_t)0x32U /**< Offset Z-axis hard-iron compensation Bit 15..8 (r/w)**/

#define LSM6DSM_A_WRIST_TILT_LAT         (uint8_t)0x50U /**< Absolute Wrist Tilt latency (r/w) **/
#define LSM6DSM_A_WRIST_TILT_THS         (uint8_t)0x54U /**< Absolute Wrist Tilt threshold (r/w)**/
#define LSM6DSM_A_WRIST_TILT_MASK        (uint8_t)0x59U /**< Absolute Wrist Tilt mask (r/w)**/

/*! @} */

/*********** FUNCTIONS ***********/

void lsm6dsm_test();

int lsm6dsm_enable_accel();
int lsm6dsm_write_buffer(uint8_t reg, uint8_t data);
int lsm6dsm_read_buffer(uint8_t reg, uint8_t *out_data);
int lsm6dsm_read_status(uint8_t *status_data);
int lsm6dsm_read_accel(float *out_xyz);
int lsm6dsm_read_gyro(float *out_xyz);

#endif // LSM6DSM