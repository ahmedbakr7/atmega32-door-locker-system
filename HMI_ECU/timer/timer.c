

#include "timer.h"
#include <vcruntime.h>

static volatile void (*g_timer1callBack_A)(void) = NULL_PTR;     // will use this function in timer1 overflow interrupt

// ISR(TIMER1_OVF_vect){
//    (*g_timer1callBack_A)(); /* another method to call the function using pointer to function g_callBackPtr(); */
// }

volatile uint16 timer1_count=0;

ISR(TIMER1_COMPA_vect){
    timer1_count++;
    
    if(g_timer1callBack_A!=NULL_PTR)
        (*g_timer1callBack_A)();
}


void Timer1_Init(const Timer1_ConfigType * timer)
{
    SREG|=(1<<7);
    TCCR1B= ((TCCR1B)& (~7)) | timer->prescaler;
    TCCR1A= (1<<FOC1A);

	TCNT1 = timer->initial_value;
    
    if(timer->mode==OVERFLOW)
        TIMSK|=(1<<TOIE1);

    else if (timer->mode==TIMER1_CTC)
    {
		OCR1A = timer->compare_value;
        TCCR1B|=(1<<WGM12);
        TIMSK|=(1<<OCIE1A);
    }


    // TCCR1A = (((timer->mode&1)||timer->mode==TIMER1_FAST_PWM_ICR1)?0:1<<FOC1A)|(((timer->mode&1)||timer->mode==TIMER1_FAST_PWM_ICR1)?0:1<<FOC1B)
    // | ((timer->mode&1)<<WGM10) | ((timer->mode&0b10)<<WGM11) ;
    // if (timer->mode==OVERFLOW)
    //     TIMSK|=(1<<TOIE1);
    // TCCR1B |= (timer->clock<<CS10);
}

void Timer1_deInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
    TIMSK=0;
    timer1_count=0;
}


void Timer1_setCallBack(void(*a_ptr)(void))
{
    g_timer1callBack_A=a_ptr;
}





void PWM_Timer0_Start(uint8 dutyCycle)
{
    OCR0= TIMER0_MAX*(dutyCycle/100.0);
    GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
    // SET_BIT(TCCR0, FOC0);
    SET_BIT(TCCR0, WGM01);    // PWM MODE
    SET_BIT(TCCR0, WGM00);
    SET_BIT(TCCR0, COM01);   // NON INVERTING
    SET_BIT(TCCR0, CS01);   // prescaler 8
}


