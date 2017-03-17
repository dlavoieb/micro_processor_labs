#ifndef LED_H__
#define LED_H__

#include <stm32f4xx_hal.h>
/** @defgroup LED 
  * Utilities to manage onboard LEDs
  * @{
  */ 

/**
 * @brief Type to select target led by color
 */
typedef enum 
{
	LED_GREEN = 1,
	LED_ORANGE = 2,
	LED_RED = 3,
	LED_BLUE = 4
}LED;

/**
 * @brief Initialize GPIO modules for onboard leds to act as standart toggles
 */
void init_internal_led_gpio_toggles(void);

/**
 * @brief Initialize led to function with PWM dimmer
 */
void init_pwm_led(void);

/**
 * @brief Sets registers to reflect a certain brightness
 * @param percent The brigthness level where 0 of OFF and 100 is ON
 * @param led The desired LED to apply the change to
 */
void set_duty_cycle_percent(float percent, LED led);

/**
 * @brief Function wrapper to toggle the LEDs.
 * @param state Select if the led should be on or off
 * @param led Select which pin we control
 */
void set_internal_led(GPIO_PinState state, uint16_t led);


/** @defgroup Macros
  * @brief Group of macros to toggle the onboard LEDs
  * @{
  */ 
#define LED_RED_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_14);		///< Turn on Red LED
#define LED_RED_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_14);		///< Turn off Red LED
#define LED_BLUE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_15);		///< Turn on Blue LED
#define LED_BLUE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_15);	///< Turn off Blue LED
#define LED_GREEN_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_12);		///< Turn on Green LED
#define LED_GREEN_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_12);	///< Turn off Green LED
#define LED_ORNAGE_ON_ set_internal_led(GPIO_PIN_SET, GPIO_PIN_13);		///< Turn on Orange LED
#define LED_ORANGE_OFF_ set_internal_led(GPIO_PIN_RESET, GPIO_PIN_13);	///< Turn off Orange LED
/**
  * @}
  */
/**
  * @}
  */

#endif // !LED_H__
