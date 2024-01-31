/*
 * buzzer.h
 *
 *  Created on: Oct 24, 2023
 *      Author: Ahmed Bakr
 */

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_

#include "../gpio/gpio.h"
#include "../std_types.h"
#include "../common_macros.h"


#define BUZZER_PORT	PORTB_ID
#define BUZZER_PIN	PIN4_ID
// #define BUZZER_POWER_ON LOGIC_HIGH


void Buzzer_init(void);

void Buzzer_on(void);

void Buzzer_off(void);


#endif /* HAL_BUZZER_BUZZER_H_ */
