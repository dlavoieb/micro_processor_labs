#ifndef BUTTON_H_
#define BUTTON_H_

/** @defgroup Button 
  * Utilities to manage onboard button
  * @{
  */ 
#include <stm32f4xx_hal.h>

/**
 * @brief Initialises GPIO pin for reading onboard button
 */
void init_button_gpio(void);

/**
 * @brief Get status of the internal button
 * @return Current state of the button pin
 */
GPIO_PinState read_button(void);

/**
  * @}
  */
#endif // !BUTTON_H_
