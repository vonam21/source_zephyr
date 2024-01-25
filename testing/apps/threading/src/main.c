/*
 * Copyright (c) 2023 Quasal
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

#define LED0_TURN_ON_PERIOD_MS 10000
#define LED0_TURN_OFF_PERIOD_MS 10000
#define LED1_TURN_ON_PERIOD_MS 15000
#define LED1_TURN_OFF_PERIOD_MS 5000
#define LED2_TURN_ON_PERIOD_MS 2000
#define LED2_TURN_OFF_PERIOD_MS 18000

#define LED1_NODE DT_NODELABEL(green_led_1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#define LED2_NODE DT_NODELABEL(green_led_2)
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#define LED3_NODE DT_NODELABEL(green_led_3)
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

void led1_control_thread(void *, void *, void *) {
    int ret;

    if (!gpio_is_ready_dt(&led1)) {
            return;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
            return;
    }

    while (1) {
        gpio_pin_set_dt(&led1, 1);
        k_msleep(LED0_TURN_ON_PERIOD_MS);
        gpio_pin_set_dt(&led1, 0);
        k_msleep(LED0_TURN_OFF_PERIOD_MS);
    }
}
void led2_control_thread(void *, void *, void *) {
    int ret;

    if (!gpio_is_ready_dt(&led2)) {
            return;
    }

    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
            return;
    }

    while (1) {
        gpio_pin_set_dt(&led2, 0);
        k_msleep(LED1_TURN_ON_PERIOD_MS);
        gpio_pin_set_dt(&led2, 1);
        k_msleep(LED1_TURN_OFF_PERIOD_MS);
    }
}
void led3_control_thread(void *, void *, void *) {
    int ret;

    if (!gpio_is_ready_dt(&led3)) {
            return;
    }

    ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
            return;
    }

    while (1) {
        gpio_pin_set_dt(&led3, 0);
        k_msleep(LED2_TURN_ON_PERIOD_MS);
        gpio_pin_set_dt(&led3, 1);
        k_msleep(LED2_TURN_OFF_PERIOD_MS);
    }
}

K_THREAD_STACK_DEFINE(led1_thread_stack_area, MY_STACK_SIZE);
struct k_thread led1_thread_data;
k_tid_t led1_threadid;
/////************************THREAD 2//////////////////////////
K_THREAD_STACK_DEFINE(led2_thread_stack_area, MY_STACK_SIZE);
struct k_thread led2_thread_data;
k_tid_t led2_threadid;
////////////////////////////////////////////////////
/////************************THREAD 3//////////////////////////
K_THREAD_STACK_DEFINE(led3_thread_stack_area, MY_STACK_SIZE);
struct k_thread led3_thread_data;
k_tid_t led3_threadid;
////////////////////////////////////////////////////
int main(void)
{
    led1_threadid = k_thread_create(&led1_thread_data, led1_thread_stack_area,
        K_THREAD_STACK_SIZEOF(led1_thread_stack_area),
        led1_control_thread,
        NULL, NULL, NULL,
        MY_PRIORITY, 0, K_NO_WAIT);
    led2_threadid = k_thread_create(&led2_thread_data, led2_thread_stack_area,
        K_THREAD_STACK_SIZEOF(led2_thread_stack_area),
        led2_control_thread,
        NULL, NULL, NULL,
        6, 0, K_NO_WAIT);
    led3_threadid = k_thread_create(&led3_thread_data, led3_thread_stack_area,
        K_THREAD_STACK_SIZEOF(led3_thread_stack_area),
        led3_control_thread,
        NULL, NULL, NULL,
        7, 0, K_NO_WAIT);
    k_thread_start(led1_threadid);
    k_thread_start(led2_threadid);
    k_thread_start(led3_threadid);
    while (1) {
        k_msleep(1000);
    }
}