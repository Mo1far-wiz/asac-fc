#include "led.h"
#include "machine.h"
#include "asac_fc.h"

#define GREEN_OFF_RED_OFF() {gpio_put(PIN_LED_GREEN, 0); gpio_put(PIN_LED_RED, 0);}
#define GREEN_ON_RED_OFF()  {gpio_put(PIN_LED_GREEN, 1); gpio_put(PIN_LED_RED, 0);}
#define GREEN_OFF_RED_ON()  {gpio_put(PIN_LED_GREEN, 0); gpio_put(PIN_LED_RED, 1);}
#define GREEN_ON_RED_ON()   {gpio_put(PIN_LED_GREEN, 1); gpio_put(PIN_LED_RED, 1);}
#define DEFAULT_LED_PIN_ON()   {gpio_put(PICO_DEFAULT_LED_PIN, 1);}
#define DEFAULT_LED_PIN_OFF()   {gpio_put(PICO_DEFAULT_LED_PIN, 0);}


int led_init() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_init(PIN_LED_GREEN);
    gpio_init(PIN_LED_RED);
    gpio_init(PIN_LED_BLUE);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_set_dir(PIN_LED_GREEN, GPIO_OUT);
    gpio_set_dir(PIN_LED_RED, GPIO_OUT);
    gpio_set_dir(PIN_LED_BLUE, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    gpio_put(PIN_LED_GREEN, 0);
    gpio_put(PIN_LED_RED, 0);
    gpio_put(PIN_LED_BLUE, 0);

    return 0;
}

void led_set(uint8_t pin, uint8_t value) {
    gpio_put(pin, value);
}


void led_run_boot_sequence() {
    for (int i = 0; i < 3; i++) {
        DEFAULT_LED_PIN_ON();
        //printf("led.c\t-32: GREEN_ON_RED_OFF\n");
        GREEN_ON_RED_OFF();
        sleep_ms(150);
        DEFAULT_LED_PIN_OFF();
        //printf("led.c\t-35: GREEN_OFF_RED_ON\n");
        GREEN_OFF_RED_ON();
        sleep_ms(150);
    }
        //printf("led.c\t-39: GREEN_OFF_RED_OFF\n");
    GREEN_OFF_RED_OFF();
    DEFAULT_LED_PIN_OFF();
}