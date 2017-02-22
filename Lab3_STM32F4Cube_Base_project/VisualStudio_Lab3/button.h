#ifndef BUTTON_H_
#define BUTTON_H_

#include <stm32f4xx_hal.h>

/**
 * @brief Initialises GPIO pins for reading onboard button
 */
void init_button_gpio(void);

/**
 * @brief Get status of the internal button
 * @return Current state of the button pin
 */
GPIO_PinState read_button(void);

/**
 * @brief Initialize GPIO modules for onboard leds
 */
void init_internal_led_gpio(void);

/**
 * @brief Function wrapper to toggle the LEDs.
 * @param state Select if the led should be on or off
 * @param led Select which pin we control
 */
void set_internal_led(GPIO_PinState state, uint16_t led);

/** @defgroup Macro definitions to toggle on-board LEDs
  * @{
  */ 
#define LED_RED_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_14);
#define LED_RED_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_14);
#define LED_BLUE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_15);
#define LED_BLUE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_15);
#define LED_GREEN_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_12);
#define LED_GREEN_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_12);
#define LED_ORNAGE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_13);
#define LED_ORANGE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_13);
/**
  * @}
  */
#endif // !BUTTON_H_
