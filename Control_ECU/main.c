#include "DcMotor/motor.h"
#include "buzzer/buzzer.h"
#include "twi/twi.h"
#include "uart/uart.h"
#include <string.h>
#include "eeprom/external_eeprom.h"
#include "util/delay.h"
#include <avr/io.h>

/******************** definitions ************************/
#define FAILED 				0
#define SUCCESS 			1

#define PASSWORD_ADDRESS  	0x10
#define PASSWORD_SIZE		5


typedef enum {
	VALIDATE_PASSWORD=0XF0,REGISTER,PASSWORD_EXISTS,LOCK_SYSTEM,OPEN_DOOR,CHANGE_PASSWORD
}STATE;


/******************** global vars ************************/

uint8 userPassword[PASSWORD_SIZE];

/******************** Functions  ************************/

uint8 getPassword(void);
uint8 writePassword(uint8 * password);


void motorTiming(void)
{	
	switch (timer1_count) {
		case 30:
			DcMotor_Rotate1(STOP, 25);
		break;
		case 36:
			DcMotor_Rotate1(ANTI_CLOCKWISE, 25);
		break;
		case 66:
			DcMotor_Rotate1(STOP, 25);
			Timer1_deInit();
		break;
	}
}

int main(void)
{
	TWI_ConfigType twi={2,TWI_400Kbps_BITRATE};
	UART_ConfigType uart_configs = {9600,DISABLED,ONE_BIT,BIT_8};
    UART_init(&uart_configs);	TWI_init(&twi);
	Timer1_ConfigType timer={0,60000,CLK_64,TIMER1_CTC};
	Timer1_setCallBack(motorTiming);
	DcMotor_Init();
	Buzzer_init();

	char pass1[6];
	char pass2[6];	
	/**************************** syncronization ************************/
	UART_recieveByte();
	UART_sendByte(0);

	/**************************************************************************/

	for(;;)
	{
		switch (UART_recieveByte()) 
		{
			case VALIDATE_PASSWORD:
				UART_receiveString(pass1);
				UART_sendByte((getPassword())&&(strcmp(userPassword, pass1)==0));
				break;

			case REGISTER:
				UART_receiveString(pass1);
				UART_receiveString(pass2);
				UART_sendByte(strcmp(pass1, pass2)==0?writePassword(pass1):FAILED);
				break;

			case PASSWORD_EXISTS:
				UART_sendByte(getPassword());
				break;
			
			case OPEN_DOOR:
				UART_sendByte(SUCCESS);
				DcMotor_Rotate1(CLOCKWISE, 25);
				Timer1_Init(&timer);
				break;

			case LOCK_SYSTEM:
				Buzzer_on();
				_delay_ms(60000);
				Buzzer_off();
				break;
		
			
		}
	}
}

uint8 getPassword(void)
{
	for (uint8 i=0;i<PASSWORD_SIZE;i++) 
	{
		// if failed to get byte or the byte isnt a number return failed
		if((!EEPROM_readByte(PASSWORD_ADDRESS+i, &userPassword[i]))|| !(userPassword[i]>='0'&&userPassword[i]<='9'))
			return FAILED;
		_delay_ms(10);
	}
	return SUCCESS;
}

uint8 writePassword(uint8 * password)
{
	for (uint8 i=0;i<PASSWORD_SIZE;i++) 
	{
		// if failed to get byte or the byte isnt a number return failed
		EEPROM_writeByte(PASSWORD_ADDRESS+i,password[i]);
		_delay_ms(10);
	}
	// strcpy(userPassword, password);
	return SUCCESS;
}


