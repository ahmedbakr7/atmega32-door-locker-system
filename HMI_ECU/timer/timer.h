#pragma once
#include "../std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../common_macros.h"
#include "../gpio/gpio.h"


#define TIMER0_MAX 0XFF
#define TIMER1_MAX 0XFFFF

typedef enum {
    NOCLOCK,
    CLK_1,
    CLK_8,
    CLK_64,
    CLK_256,
    CLK_1024,
    FALLING,    // external clock on T0 pin falling edge
    RISING      // external clock on T0 pin rising edge
}Timer1_Prescaler;

typedef enum {
    OVERFLOW,
    TIMER1_CTC=0B100,
    TIMER1_FAST_PWM_8BIT=0B101,
    TIMER1_CTC_ICR1=0B1100,
    TIMER1_FAST_PWM_ICR1=0B1110,
}Timer1_Mode;


typedef struct {
    uint16 initial_value;
    uint16 compare_value; // it will be used in compare mode only.
    Timer1_Prescaler prescaler;
    Timer1_Mode mode;
}Timer1_ConfigType;

extern volatile uint16 timer1_count;


void Timer1_Init(const Timer1_ConfigType * timer);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));


void PWM_Timer0_Start(uint8 dutyCycle);


