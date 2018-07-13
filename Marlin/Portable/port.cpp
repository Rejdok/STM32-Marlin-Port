#include <port.h>

extern TIM_HandleTypeDef TimerStepper;

#ifdef STM32F407xx
uint32_t FREQU = 168000000;
#endif
#ifdef STM32F103xB
uint32_t FREQU = 76000000;
#endif
/*Returns CPU Frequency*/
uint32_t portGetCPUFrequency() {
	uint32_t rez = HAL_RCC_GetSysClockFreq();
	//if ( rez == 168000000 ) {
		return rez;
	//}/*Just for debugg*/
	//while ( 1 ) {
	//}
}

/*Todo ��������� ������� ������� ������ ������... �������� ��� � �������
 * ���������, ��� ������� �������� ����� � �������*/

/*TODO stepper pins definitions*/
//struct pinDefenition {
//		GPIO_TypeDef* port;
//		uint16_t pin;
//};

#define steppersCount 5

#define CHECK(PORT, PIN) PORT ## PIN
#define PIN(PORT, PIN) (PROT, PIN)

/*todo add heater an fan pins*/
#ifdef STM32F103xB
extern pinDefenition pinMap[];
extern int portPinCount;
#else
pinDefenition pinMap[] = {
/*Stepper pins order:
 * STEP 			-> DIR 				-> ENABLE*/
//{ GPIOD, GPIO_PIN_14 }, { GPIOD, GPIO_PIN_15 }, { GPIOG, GPIO_PIN_2 }, /*X axis motor*/
//{ GPIOG, GPIO_PIN_3 }, { GPIOG, GPIO_PIN_4 }, { GPIOG, GPIO_PIN_5 }, /*Y axis motor*/
//{ GPIOG, GPIO_PIN_6 }, { GPIOG, GPIO_PIN_7 }, { GPIOG, GPIO_PIN_8 }, /*Z axis motor*/
{ GPIOE, GPIO_PIN_7 }, { GPIOE, GPIO_PIN_8 }, { GPIOE, GPIO_PIN_9 }, /*Extruder 1 motor*/
{ GPIOE, GPIO_PIN_10 }, { GPIOE, GPIO_PIN_11 }, { GPIOE, GPIO_PIN_12 }, /* Extruder 2 motor*/
{ GPIOE, GPIO_PIN_13 }, /*heater 0*/
{ GPIOE, GPIO_PIN_14 } /*FAN */
};
#define portPinCount (sizeof(pinMap)/sizeof(pinMap[0]))
#endif

bool digitalRead(uint8_t pin) {
	uint8_t curpin = pin - 1;
	if ( curpin < portPinCount )
		return HAL_GPIO_ReadPin(pinMap[curpin].port, pinMap[curpin].pin);
	else
		return 0;
}
void digitalWrite(uint8_t pin, uint8_t state) {
	uint8_t curpin = pin - 1;
	GPIO_PinState a = ( state == 0 ) ? GPIO_PIN_RESET : GPIO_PIN_SET;
	if ( state < 2 )
		if ( curpin < portPinCount )
			HAL_GPIO_WritePin(pinMap[curpin].port, pinMap[curpin].pin, a);
}
void analogWrite(uint8_t pin, uint8_t value) {

}
/*updates timer period*/
void stepperTimerPeriodUpdate(uint32_t value) {
	if ( value <= UINT16_MAX ) {
		TimerStepper.Init.Period = value;
		TimerStepper.Instance->ARR = value;
	}
}


uint16_t getTimerValue(TIM_HandleTypeDef *timer){
	return timer->Instance->CNT;
}
/*math functions*/
//template <class T>
//T sq(T a){
//	return a*a;
//}
//template <class T> const T max (const T a, const T b) {
//  return (a<b)?b:a;
//}
//template <class T> const T min (const T a, const T b) {
//  return (a<b)?a:b;
//};
//template<class T> T constrain( T x, T a, T b){
//	if(x<a)
//		return a;
//	if(x>b)
//		return b;
//	return x;
//};
