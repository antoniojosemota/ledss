#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/aon_timer.h"

#define BTN_A 5
#define BTN_B 6
#define LED_G 11
#define LED_B 12
#define LED_R 13

struct timespec inicio = {.tv_sec = 0, .tv_nsec = 0};

void setup(){
    stdio_init_all();

    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_init(LED_R);

    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);

    gpio_put(LED_B, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_R, 0);
}

void callback_A(){
    aon_timer_start(&inicio);
    gpio_put(LED_R, 1);
}

void callback_B(){
    aon_timer_stop();
    gpio_put(LED_R, 0);
    gpio_put(LED_B, 1);
    sleep_ms(5000);
    gpio_put(LED_R, 0);

}

int main()
{
    stdio_init_all();

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &callback_A);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &callback_B);

    while (true) {
        uint64_t elapsed_us = aon_timer_get_time(&inicio);
        elapsed_us *= 1000;
        printf("Tempo: %d segundos\n", elapsed_us);
        sleep_ms(1000);
    }
}
