#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/Timer.h"
#include "../Drivers/LCD.h"
#include "../Drivers/RTC.h"
#include "../Drivers/types.h"
#include "../Drivers/string.h"
#include "../Drivers/varios.h"
#include "../Drivers/UART.h"
#include "Aplicacion.h"


extern uint8_t ledStatus;
extern LCD_t LCD;

menu_t menu = {
		.level = 0,
		.pos = {0,0},
		.op[0].msg = "CHANGE OUTPUT",
		.op[0].desc = "Turn the state of an output. Hold to enter.",
		.op[0].sub_op[0].msg = "switch LED0",
		.op[0].sub_op[1].msg = "switch LED1",
		.op[0].sub_op[2].msg = "switch LED2",
		.op[0].sub_op[3].msg = "switch LED3",
		.op[1].msg = "opt1",
		.op[1].desc = "opt1 desc. Hold to enter.",
		.op[2].msg = "opt2",
		.op[2].desc = "opt2 desc. Hold to enter."

};

void showClock()
{
	if(LCD.isInClock == FALSE && LCD.isOn == TRUE)
	{
		LCD.scroll.isScrolling = FALSE;
		LCD_displayClock();
		LCD.isInClock = TRUE;
	}
}

void restoreScreen()
{
	LCD.scroll.isScrolling = FALSE;
	LCD.isInMenu = FALSE;
//	if(LCD.isInClock == TRUE)
		LCD_displayClock();
//	else
//		LCD_clear();
	LCD.isOn = TRUE;
	menu.pos[0] = 0;
	menu.pos[1] = 0;
}

void ledBlink()
{
	static bool st = TRUE;
	if(st)
	{
		write_pin(LEDLPC_G, LEDLPC_OFF);
		startTimer(4900, ledBlink);
	}
	else
	{
		startTimer(100, ledBlink);
		write_pin(LEDLPC_G, LEDLPC_ON);
	}
	st = !st;
}


void showMenu()
{



	if(menu.level == 0)
	{
		LCD_printCentered(menu.op[menu.pos[0]].msg , LCD_ROW_1);
		LCD_scrollMessage(menu.op[menu.pos[0]].desc, LCD_ROW_2);
	}
	else if(menu.level == 1)
	{
		LCD.scroll.isScrolling = FALSE;
		LCD_clear();
		LCD_printCentered(menu.op[menu.pos[0]].sub_op[menu.pos[1]].msg , LCD_ROW_1);
	}

	startnTimer(6, 20000, restoreScreen);
}

void enterMenu()
{
	if(menu.level == 0)
	{
		menu.level++;
	}
	else if(menu.level == 1)
	{
		switch(menu.pos[0])
		{
		case 0:
			toggleLed(menu.pos[1]);
			break;
		}
	}
	showMenu();
}


/*		if(ledStatus)
		{
			ledOFF();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_print("Led OFF");
				startnTimer(7, 1000, showClock);
			}
		}
		else
		{
			ledON();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_print("Led ON");
				startnTimer(7, 1000, showClock);
			}
		}*/
void SW1_handler(uint8_t st)
{
	if(st)
	{
		startnTimer(5, 600, enterMenu);
	}
	else
	{
		if(!isTimerEnd(5))
		{
			if(LCD.isInMenu)
			{
				menu.pos[menu.level]++;
				if(menu.level == 0)
					menu.pos[menu.level] %= 3;
				else if(menu.level == 1)
					menu.pos[menu.level] %= 4;
			}
			stopTimer(5);
			showMenu();
		}
		LCD.isInMenu = TRUE;
		LCD.isInClock = FALSE;
	}
}

void SW2_handler(uint8_t st)
{
	if(st)
	{
		if(LCD.isInMenu)
		{
			if(menu.level>0)
			{
				menu.level--;
				showMenu();
			}
			else
			{
				restoreScreen();
			}
		}
	}
}

void SW3_handler(uint8_t st)
{
	if(st)
	{
		if(LCD.isInClock)
		{
			//LCD_OFF();
			LCD.isOn = FALSE;
			LCD.isInClock = FALSE;
			LCD_clear();
		}
		else
		{
		//	LCD_ON();
			LCD.isOn = TRUE;
			LCD.isInClock = TRUE;
			LCD_displayClock();
		}
	}
}
/*
void SW3_handler(uint8_t st)
{
	if(st)
	{
		if(ledStatus)
		{
			ledUP();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_printCentered("Swap LED", LCD_ROW_1);
				startnTimer(7, 2000, showClock);
			}
		}
		else
		{
			LCD.isOn = FALSE;
			LCD_clear();
			int err = 0, msjs=0;
			char str[17] = "send()   err:";
			char aux[4];
			for(int i=0; i < BUFFER_TX_SIZE/2; i++) {
				err += UART0_sendChar(0x55);
				err += UART0_sendChar(0xAA);
				msjs += 2;
			}
			LCD_clear();
			LCD_printUP(strcat(str, intToStr(err, aux, 1)));
			strcpy(str,"buf:");
			strcat(str, intToStr(BUFFER_TX_SIZE, aux, 1));
			strcat(str," msjs:");
			strcat(str, intToStr(msjs, aux, 1));
			LCD_printDOWN(str);

			startTimer(8000, restoreScreen);
		}
	}
}
*/
void SW4_handler(uint8_t st) {}

#if _5_ENTRADAS
void SW5_handler(uint8_t st) {}
#endif






