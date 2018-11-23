/**
 * @file blinky.c
 *
 * Blink an LED.
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"

// ESP-12 modules have LED on GPIO2. Change to another GPIO
// for other boards.
static const int pin = 2;
static volatile os_timer_t some_timer;

// Non-OTA partition table; see Section 4 of the ESP8266 SDK Getting
// Started Guide
static const partition_item_t partition_table[] = {
    {EAGLE_FLASH_BIN_ADDR, 0x00000, 0x10000},
    {EAGLE_IROM0TEXT_BIN_ADDR, 0x10000, 0x60000},
    {SYSTEM_PARTITION_RF_CAL, SYSTEM_PARTITION_RF_CAL_ADDR, 0x1000},
    {SYSTEM_PARTITION_PHY_DATA, SYSTEM_PARTITION_PHY_DATA_ADDR, 0x1000},
    {SYSTEM_PARTITION_SYSTEM_PARAMETER, SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 0x3000}
};

void some_timerfunc(void *arg)
{
    // do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1 << pin)) {
        gpio_output_set(0, (1 << pin), 0, 0);  // set gpio low
    } else {
        gpio_output_set((1 << pin), 0, 0, 0);  // set gpio high
    }
}

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if (!system_partition_table_regist(partition_table, sizeof(partition_table)/sizeof(partition_table[0]), SPI_FLASH_SIZE_MAP)) {
        os_printf("system_partition_table_regist fail\r\n");
        while(1);
    }
}

void ICACHE_FLASH_ATTR user_init()
{
  // init gpio subsytem
  gpio_init();

  // configure UART TXD to be GPIO1, set as output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);
  gpio_output_set(0, 0, (1 << pin), 0);

  // setup timer (500ms, repeating)
  os_timer_setfn((os_timer_t *) &some_timer, (os_timer_func_t *) some_timerfunc, NULL);
  os_timer_arm((os_timer_t *) &some_timer, 500, 1);
}
