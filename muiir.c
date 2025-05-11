#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BTN_A 5
#define BTN_B 6
#define LED_G 11
#define LED_B 12
#define LED_R 13

volatile bool running = false;
absolute_time_t start_time;
int64_t elapsed_us = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_A && (events & GPIO_IRQ_EDGE_FALL)) {
        if (!running) {
            running = true;
            start_time = get_absolute_time();
            gpio_put(LED_R, 1);  // Liga LED vermelho
            gpio_put(LED_B, 0);
            gpio_put(LED_G, 0);
        }
    }

    if (gpio == BTN_B && (events & GPIO_IRQ_EDGE_FALL)) {
        if (running) {
            running = false;
            gpio_put(LED_R, 0);  // Desliga LED vermelho
            gpio_put(LED_B, 1);  // Liga LED azul
            printf("Tempo: %.2f segundos\n", elapsed_us / 1e6);
            gpio_put(LED_B, 0);
        }
    }
}

void setup() {
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

    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);
    gpio_put(LED_R, 0);

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_B, GPIO_IRQ_EDGE_FALL, true);
}

int main() {
    setup();

    while (true) {
        if (running) {
            elapsed_us = absolute_time_diff_us(start_time, get_absolute_time());
        }
        sleep_ms(1000);
    }
}
