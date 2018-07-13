typedef unsigned int bool;

#include <port.h>

#ifdef STM32F103xB
pinDefenition pinMap[] = {
/*Stepper pins order:
 * STEP 			-> DIR 				-> ENABLE*/
{ GPIOA, GPIO_PIN_1 }, { GPIOA, GPIO_PIN_2 }, { GPIOA, GPIO_PIN_3 }, /*X axis motor*/
{ GPIOA, GPIO_PIN_4 }, { GPIOA, GPIO_PIN_5 }, { GPIOA, GPIO_PIN_6 }, /*Y axis motor*/
{ GPIOB, GPIO_PIN_10 }, { GPIOB, GPIO_PIN_11 }, { GPIOB, GPIO_PIN_12 }, /*Z axis motor*/
{ GPIOB, GPIO_PIN_13 }, { GPIOB, GPIO_PIN_14 }, { GPIOB, GPIO_PIN_15 }, /*Extruder 1 motor*/
{ GPIOB, GPIO_PIN_3 }, { GPIOB, GPIO_PIN_4 }, { GPIOB, GPIO_PIN_5 }, /* Extruder 2 motor*/
{ GPIOA, GPIO_PIN_7 }, /*heater 0*/
{ GPIOB, GPIO_PIN_0 } /*FAN */
};

int portPinCount = (sizeof(pinMap)/sizeof(pinMap[0]));
#endif

