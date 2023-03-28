#include <ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
// #include <ztest_test_deprecated.h>
#include "max30003.h"
#include "lsm6dsm.h"
#include "spi_config.h"
#include "utils.h"

char accel[16] = "lsm6dsm_test";
char ecg[16] = "max30003_test";

#define WRITE_BIT_SET(reg) (reg & ~(1 << 7))
#define READ_BIT_SET(reg) (reg | (1 << 7))

extern uint8_t buffer_tx[BUF_SIZE];
extern uint8_t buffer_rx[BUF_SIZE];
extern uint8_t buffer_print_tx[BUF_SIZE * 5 + 1];
extern uint8_t buffer_print_rx[BUF_SIZE * 5 + 1];
extern struct spi_buf tx_bufs[1];
extern struct spi_buf rx_bufs[1];
// extern const struct spi_buf_set tx;
// extern const struct spi_buf_set rx;

static void test_lsm6dsm_read_buffer(void)
{
    uint8_t data;

    int lsm6dsm_r_ret = lsm6dsm_read_buffer(0x0F, &data);
    zassert_equal(0, lsm6dsm_r_ret, "lsm6dsm function call failed");

    to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
    to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
    // zassert_equal(0,data,"data is %x",data);
    zassert_equal(0x6A, buffer_rx[1], "Buffer content mismatch, TX Buffer:  %s\n RX Buffer:  %s\n ", buffer_print_tx, buffer_print_rx);
}

static void test_max30003_read(void)
{
    uint8_t data[3];
    int max30003_r_ret = max30003_read(INFO, data, sizeof data);
    uint8_t data_shift = data[0] & 0xF0;
    zassert_equal(0, max30003_r_ret, "max30003 function call failed");

    to_display_format(buffer_rx, BUF_SIZE, buffer_print_rx);
    to_display_format(buffer_tx, BUF_SIZE, buffer_print_tx);
    zassert_equal(0x50, data_shift, "Buffer content mismatch, TX Buffer:  %s\n RX Buffer:  %s\n ", buffer_print_tx, buffer_print_rx);
}

// define the test suite and test cases
void test_main(void)
{
    ztest_test_suite(ECG_test_suite,
                     ztest_unit_test(test_lsm6dsm_read_buffer), ztest_unit_test(test_max30003_read));

    // run the test suite
    ztest_run_test_suite(ECG_test_suite);
}