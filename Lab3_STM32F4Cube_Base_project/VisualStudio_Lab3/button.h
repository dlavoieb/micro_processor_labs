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

#endif // !BUTTON_H_
