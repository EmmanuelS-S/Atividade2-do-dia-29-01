#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_BLUE 11
#define LED_RED 12
#define LED_GREEN 13
#define BUTTON_PIN 5

volatile bool button_pressed = false;
volatile bool sequence_running = false;

// Função de debounce para o botão
bool debounce_button() {
    static absolute_time_t last_press = 0;
    absolute_time_t now = get_absolute_time();

    if (absolute_time_diff_us(last_press, now) < 300000) { // 300 ms
        return false;
    }

    last_press = now;
    return true;
}

// Função callback para desligar LEDs em sequência
int turn_off_callback(alarm_id_t id, void *user_data) {
    static int state = 0;

    switch (state) {
        case 0:
            gpio_put(LED_GREEN, 0);
            state++;
            break;
        case 1:
            gpio_put(LED_RED, 0);
            state++;
            break;
        case 2:
            gpio_put(LED_BLUE, 0);
            sequence_running = false; // Sequência concluída
            state = 0; // Reiniciar para o próximo ciclo
            break;
    }

    return 0; // Finalizar alarme
}

// Função de interrupção do botão
void button_callback(uint gpio, uint32_t events) {
    if (!sequence_running && debounce_button()) {
        button_pressed = true;
    }
}

int main() {
    // Inicialização de GPIOs
    stdio_init_all();

    gpio_init(LED_BLUE);
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(BUTTON_PIN);

    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    while (true) {
        if (button_pressed) {
            button_pressed = false;
            sequence_running = true;

            // Acender todos os LEDs
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);

            // Configurar temporizadores para desligar os LEDs
            add_alarm_in_ms(3000, turn_off_callback, NULL, true);
            add_alarm_in_ms(6000, turn_off_callback, NULL, true);
            add_alarm_in_ms(9000, turn_off_callback, NULL, true);
        }
    }

    return 0;
}