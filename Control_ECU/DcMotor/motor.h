#pragma once
#include "../gpio/gpio.h"
#include "../common_macros.h"
#include "../std_types.h"
#include <avr/io.h>
#include "../timer/timer.h"

#define DCMOTOR_TWO_MOTORS_INCLUDED 0               // set to 1, if two motors are being used on the same driver

#define DCMOTOR_ENABLE_LOGIC_HIGH LOGIC_LOW        // to set the activation power for the enables of the motor driver, notice that u can change each enable pin separately

#define DCMOTOR_INPUT1_PORT       PORTB_ID          // port holding input1,input2
#define DCMOTOR_INPUT1_FIRST_PIN  PIN1_ID           // first pin pointing to input1 then input2

#define DCMOTOR_ENABLE1_PORT   PORTB_ID             // port holding input1,input2
#define DCMOTOR_ENABLE1_PIN   PIN3_ID
#define DCMOTOR_ENABLE1_LOGIC_HIGH  DCMOTOR_ENABLE_LOGIC_HIGH
#define DCMOTOR_ENABLE1_LOGIC_LOW (!(DCMOTOR_ENABLE1_LOGIC_HIGH))


#if DCMOTOR_TWO_MOTORS_INCLUDED
#define DCMOTOR_INPUT3_PORT       PORTA_ID          // port holding input1,input2
#define DCMOTOR_INPUT3_FIRST_PIN  PIN0_ID

#define DCMOTOR_ENABLE3_PORT   PORTA_ID
#define DCMOTOR_ENABLE3_PIN   PIN2_ID
#define DCMOTOR_ENABLE3_LOGIC_HIGH  DCMOTOR_ENABLE_LOGIC_HIGH
#define DCMOTOR_ENABLE3_LOGIC_LOW (!(DCMOTOR_ENABLE3_LOGIC_HIGH))
#endif


typedef enum {
    STOP=0,
    ANTI_CLOCKWISE,
    CLOCKWISE,
}DcMotor_State;


void DcMotor_Init(void);

void DcMotor_Rotate1(DcMotor_State state, uint8 speed);

#if DCMOTOR_TWO_MOTORS_INCLUDED

void DcMotor_Rotate3(DcMotor_State state, uint8 speed);
#endif