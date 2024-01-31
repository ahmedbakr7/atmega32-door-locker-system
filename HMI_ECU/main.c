#include "lcd/lcd.h"
#include "keypad/keypad.h"
#include "uart/uart.h"
#include "util/delay.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer/timer.h"


typedef enum {
		VALIDATE_PASSWORD=0XF0,REGISTER,PASSWORD_EXISTS,LOCK_SYSTEM,OPEN_DOOR,CHANGE_PASSWORD
}State;

uint8 * getInput(uint8 *str);
uint8 login(uint8 *str);
void promptMainMenu(uint8 * pass1,uint8 * pass2);
void clearScreen(){ LCD_clearScreen();LCD_moveCursor(0, 0); }
uint8 registerPassword(uint8 * pass1,uint8 * pass2);
uint8 validatePassword(uint8 * pass1);


Timer1_ConfigType timer={0,60000,CLK_64,TIMER1_CTC};

void timercallBack(void)
{

}

int main(void)
{
	/* Initialization */
	LCD_init();
	// Timer1_setCallBack(timercallBack);


	UART_ConfigType uart_configs = {9600,DISABLED,ONE_BIT,BIT_8};
    UART_init(&uart_configs);
	/**************************** sync ************************/
	UART_sendByte(0);
	UART_recieveByte();
	/**************************** code ************************/
	// LCD_displayString("HELLO");
	uint8 pass1[7];
	uint8 pass2[7];

	UART_sendByte(PASSWORD_EXISTS);
	if (UART_recieveByte()) 		// If receive byte is true then there is a password
	{
		while(!login(pass1));          // keep trying till the user inserts the password
	}
	else {
		while(!registerPassword(pass1,pass2));		// keep trying till the user creates a correct password
	}

	for(;;)
	{
		promptMainMenu(pass1,pass2);
	}
}


uint8  login(uint8 *str)
{
	clearScreen();
	LCD_displayString("log in:");
	LCD_moveCursor(1, 0);
	getInput(str);
	UART_sendByte(VALIDATE_PASSWORD);
	UART_sendString(str);
	return UART_recieveByte();
}

uint8 * getInput(uint8 *str)
{
	uint8 i=0;
	while(i<5)
	{
		_delay_ms(400);
		char keyPressed=KEYPAD_getPressedKey();
		if ((keyPressed>=0)&&(keyPressed<=9))
		{
			str[i]= keyPressed+'0';
			LCD_displayCharacter(str[i]);
			i++;
		}
	}
	for (;;) 
	{
		if(KEYPAD_getPressedKey()=='=')
			{
				str[5]='#';
				str[6]='\0';
				clearScreen();
				return str;
			}
	}
}

void promptMainMenu(uint8 * pass1,uint8 *pass2)
{
	clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change pass");
	for (;;) 
	{
		uint8 keyPressed=KEYPAD_getPressedKey();
		uint8 i=0;
		for (; i<=2; i++) 					//	
		{
			if(validatePassword(pass1))
				break;
		}
		if (i==3) {								// lock system
			UART_sendByte(LOCK_SYSTEM);
			LCD_clearScreen();
			LCD_displayString("Error!!");
			_delay_ms(60000);
			break;
		}

		switch (keyPressed)
		{
			case '+':
				UART_sendByte(OPEN_DOOR);
				if (UART_recieveByte()) 
				{
					clearScreen();
					LCD_displayString("Door is unlocking");
					Timer1_Init(&timer);
					uint8 value=15;
					for(;;) 
					{
						LCD_moveCursor(1,0);
						LCD_intgerToString(value-timer1_count/2);
						LCD_displayString("  ");
						switch (timer1_count) 
						{
							case 30:																// door is stopping
								LCD_displayStringRowColumn(0,0,"Door is holding.. ");
								value=18;
							break;
							case 36:																// door is closing
								LCD_displayStringRowColumn(0,0,"Door is locking");
								value=33;
							break;
							case 66:															// door is closed!
								Timer1_deInit();
								return;
							break;
						}
					}
					return;
				}
				break;
			case '-':
				if (validatePassword(pass1))
				{
					clearScreen();
					LCD_displayString("success, insert");
					LCD_displayStringRowColumn(1,0,"New password:");
					_delay_ms(2000);
					while(!registerPassword(pass1, pass2));
				}
				return;
				break;

		}
	}
}

uint8 registerPassword(uint8 * pass1,uint8 * pass2)
{
	clearScreen();
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1, 0);
	getInput(pass1);
	LCD_displayString("Plz re-enter the");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass: ");
	getInput(pass2);
	UART_sendByte(REGISTER);
	UART_sendString(pass1);
	UART_sendString(pass2);
	return UART_recieveByte();
}

uint8 validatePassword(uint8 * pass1)
{
	clearScreen();
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1, 0);
	getInput(pass1);
	UART_sendByte(VALIDATE_PASSWORD);
	UART_sendString(pass1);
	return UART_recieveByte();
}
