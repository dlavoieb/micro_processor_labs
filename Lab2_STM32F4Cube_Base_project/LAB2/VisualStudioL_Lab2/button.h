#ifndef BUTTON_H_
#define BUTTON_H_

#include <stm32f4xx_hal.h>

void init_button_gpio(void);
GPIO_PinState read_button(void);
void init_internal_led_gpio(void);
void set_internal_led(GPIO_PinState state, uint16_t led);

#define LED_RED_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_14);
#define LED_RED_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_14);
#define LED_BLUE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_15);
#define LED_BLUE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_15);
#define LED_GREEN_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_12);
#define LED_GREEN_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_12);
#define LED_ORNAGE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_13);
#define LED_ORANGE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_13);

#endif // !BUTTON_H_
