#ifndef KEYPAD_H_
#define KEYPAD_H_
/** @defgroup Keypad  
 * @brief Driver for keypad
 *
 * Keypad pin mappings
 * <pre>
 * PIN1 -->	PC6
 * PIN2 -->	PC7
 * PIN3 -->	PC8
 * PIN4 -->	PC9
 * PIN5 -->	PB6
 * PIN6 -->	PB7
 * PIN7 -->	PB8
 * PIN8 -->	PB9
 * </pre>
 *
 * @{
 *
 */

/**
 * @brief Initialise GPIO related to the keypad
 */
void keypad_init(void);

/**
 * @brief Read the current pressed character on the keypad
 *
 * Perform the sweep algorithm to decode the current character on the keypad. 
 * Output sink is in function arguments, return value is used to indicate 
 * error.
 *
 * @param c Pointer to store the read character in
 * @return Error status, 0 is success, 1 if no character is read
 */
int keypad_read_char(char * c);

/**
  * @}
  */
#endif // !KEYPAD_H_
