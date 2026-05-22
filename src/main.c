#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

/* 1. Grab the hardware definitions from our app.overlay Devicetree */
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(ledred), gpios);
static const struct gpio_dt_spec led_yellow = GPIO_DT_SPEC_GET(DT_ALIAS(ledyellow), gpios);
static const struct device *const bmp280 = DEVICE_DT_GET(DT_ALIAS(tempsensor));

/* 2. Define the Finite State Machine (FSM) states */
typedef enum {
    STATE_INIT,
    STATE_READ_TEMP,
    STATE_TEMP_HIGH,
    STATE_TEMP_LOW
} system_state_t;

/* Set your temperature threshold in Celsius here */
#define TEMP_THRESHOLD 30.0  

int main(void) {
    system_state_t current_state = STATE_INIT;
    struct sensor_value temp_val;
    double current_temp = 0.0;

    while (1) {
        switch (current_state) {
            
            case STATE_INIT:
                /* Verify all hardware is detected and configured correctly */
                if (!gpio_is_ready_dt(&led_red) || !gpio_is_ready_dt(&led_yellow) || !device_is_ready(bmp280)) {
                    printk("ERROR: Hardware not ready. Check Devicetree and wiring.\n");
                    k_msleep(1000);
                    break; /* Stay in INIT until fixed */
                }
                gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);
                gpio_pin_configure_dt(&led_yellow, GPIO_OUTPUT_INACTIVE);
                
                printk("Hardware Initialized. Starting Sensor Loop...\n");
                current_state = STATE_READ_TEMP;
                break;

            case STATE_READ_TEMP:
                /* Use the Zephyr Sensor API to pull data via I2C */
                sensor_sample_fetch(bmp280);
                sensor_channel_get(bmp280, SENSOR_CHAN_AMBIENT_TEMP, &temp_val);
                current_temp = sensor_value_to_double(&temp_val);
                
                printk("Current Temperature: %.2f C\n", current_temp);

                /* Decide next state based on threshold */
                if (current_temp > TEMP_THRESHOLD) {
                    current_state = STATE_TEMP_HIGH;
                } else {
                    current_state = STATE_TEMP_LOW;
                }
                break;

            case STATE_TEMP_HIGH:
                gpio_pin_set_dt(&led_red, 1);
                gpio_pin_set_dt(&led_yellow, 0);
                printk("-> Threshold Exceeded: Red LED ON\n");
                k_msleep(1000); /* Wait 1 second before reading again */
                current_state = STATE_READ_TEMP;
                break;

            case STATE_TEMP_LOW:
                gpio_pin_set_dt(&led_red, 0);
                gpio_pin_set_dt(&led_yellow, 1);
                printk("-> Temperature Normal: Yellow LED ON\n");
                k_msleep(1000); /* Wait 1 second before reading again */
                current_state = STATE_READ_TEMP;
                break;
        }
    }
    return 0;
}