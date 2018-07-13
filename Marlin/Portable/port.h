#ifndef MARLIN_PORTABLE_LAYER
#define MARLIN_PORTABLE_LAYER

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif
#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#endif

#include "math.h"
#include "string.h"


struct _pinDefenition {
		GPIO_TypeDef* port;
		uint16_t pin;
};
typedef struct _pinDefenition pinDefenition;

/*CPU frequency*/
uint32_t portGetCPUFrequency();
extern uint32_t FREQU;
#define F_CPU FREQU
#define DIRPulseDelay 2 //In us
/*Some def from arduino*/
#define delay(x) HAL_Delay(x)
#define port_millis(x) HAL_GetTick(x)
#define LOW 0
#define HIGH 1
typedef uint8_t byte;
//typedef uint32_t bool;
typedef bool boolean;


/*Specific memory functions*/
/*Because we have too much RAM we can store tables and constants directly in RAM*/
#define PROGMEM
#define PSTR
#define pgm_read_byte(a) (*(a))
#define pgm_read_byte_near(a) (* ( (uint8_t*) a) )
#define pgm_read_word(a) (*  (a) )
#define pgm_read_word_near(a) (* ( uint8_t*) (a) ) )
#define pgm_read_float_near(a)(*( (float *) a ))
/*Specific string functions*/
#define strncpy_P(a,b,c) strncpy(a,b,c)
#define strstr_P(a,b) strstr(a,b)
#define sprintf_P(args...) sprintf(args)

/*Math functions*/
/*Calculates the square of a number: the number multiplied by itself.*/
//template<class T> T sq(T a);
//template <class T> const T min (const T a, const T b);
//template <class T> const T max (const T a, const T b);
//template<class T> T constrain(T x,T a,T b);
#define constrain(x,a,b) ( (x) < (a) ? (a) : ( (x>(b)) ? (b) : (x) ) )
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define sq(a) ((a)*(a))
/*Device specific functions*/
uint16_t getTimerValue(TIM_HandleTypeDef *timer);
void stepperTimerPeriodUpdate(uint32_t value);
bool digitalRead(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t status);

#define SET_INPUT(x)
#define WRITE(pin, status) digitalWrite(pin, status)
#define OUT_WRITE(pin,status) digitalWrite(pin, status)
#define pinMode(pinNumber, pinType)
#define SET_OUTPUT(PIN)
#define READ(x) 0
#define START_ADC(x) /*change of adc channels*/
/*��������*/
void analogWrite(uint8_t pin, uint8_t value);
#define LCD_MESSAGEPGM(x)
#define LCD_ALERTMESSAGEPGM(x)
#endif /* MARLIN_PORTABLE_LAYER */
