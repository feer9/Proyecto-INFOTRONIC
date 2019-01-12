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
#include "../Drivers/ADC.h"
#include "Aplicacion.h"

/* TODO:
	- Cuando no se esta mostrando el reloj,
	desactivar las interrupciones del RTC (segundos)
	- Opcion para reiniciar un Timer
*/

static int8_t sw1_timerId = -1;
extern uint8_t ledStatus;
extern LCD_t LCD;
extern ADC_t adc;
extern uart_t uart0;

menu_t menu = {
		.level = 0,			// 0 (menu) , 1 (submenu)
		.pos = {0,0},		// position in each level
		.op[0].msg = "1- CHANGE OUTPUT",
		.op[0].desc = "Turn the state of an output. Hold to enter.",
		.op[0].sub_op[0].msg = "switch LED0",
		.op[0].sub_op[1].msg = "switch LED1",
		.op[0].sub_op[2].msg = "switch LED2",
		.op[0].sub_op[3].msg = "switch LED3",
		.op[1].msg = "2- ADC Read",
		.op[1].desc = "get value of \"Ent Analog 0\". Hold to enter",
		.op[1].sub_op[0].msg = "AD0.5",
		.op[2].msg = "3- UART",
		.op[2].desc = "enable/send through UART0. Hold to enter.",
		.op[2].sub_op[0].msg = "Send <F0RRo>",
		.op[2].sub_op[1].msg = "set up",
		.op[2].sub_op[2].msg = "set down",
		.timerId = -1
};



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
		write_pin(LEDLPC_G, LEDLPC_ON);
		startTimer(100, ledBlink);
	}
	st = !st;
}

void restoreScreen()
{
//	menu.timerId = -1;
	LCD_stopScroll();
	LCD.isInMenu = FALSE;
	if(LCD.isInClock == TRUE)
		LCD_displayClock();
	else
		LCD_clear();
	LCD.isOn = TRUE;
	menu.pos[0] = 0;
	menu.pos[1] = 0;
}

void showClock()
{
	LCD.restore_timerId = -1;
	if(LCD.isInClock == FALSE && LCD.isOn == TRUE)
	{
		LCD_stopScroll();
		LCD_displayClock();
		LCD.isInClock = TRUE;
	}
}

void showADC()
{
	if (adc.change)
	{
		adc.change = 0;
		LCD_printInt(adc.AD5_val, LCD_ROW_2, 4);
	}
	adc.timerId = startTimer(200, showADC);
}

void stopADC()
{
	stopTimer(&adc.timerId);
	ADC_stop();
}

void showMenu()
{
	//stopTimer(&menu.timerId);
	if(menu.level == 0)
	{
		LCD_printCentered(menu.op[menu.pos[0]].msg , LCD_ROW_1);
		LCD_scrollMessage(menu.op[menu.pos[0]].desc, LCD_ROW_2);
	}
	else if(menu.level == 1)
	{
		LCD_stopScroll();
		if(menu.pos[0] == 0) // digital out
		{
			LCD_clear();
			if(D_IN_getStatus(menu.pos[1]))
				LCD_printCentered("ON", LCD_ROW_2);
			else
				LCD_printCentered("OFF", LCD_ROW_2);
		}
		else if(menu.pos[0] == 2) // uart
		{
			LCD_clear();
			if(uart0.status)
				LCD_printCentered("UART0 is up", LCD_ROW_2);
			else
				LCD_printCentered("UART0 is down", LCD_ROW_2);
		}
		LCD_printCentered(menu.op[menu.pos[0]].sub_op[menu.pos[1]].msg , LCD_ROW_1);
	}

//	menu.timerId = startTimer(20000, restoreScreen);
}

void enterMenu()
{
	sw1_timerId = -1;
	if(menu.level == 0)
	{
		if(menu.pos[0] == 1)
		{
			ADC_start();
			showADC();
		}
		menu.level++;
	}
	else if(menu.level == 1)
	{
		switch(menu.pos[0])
		{
		case 0: // DIGITAL
			D_IN_toggle(menu.pos[1]);
			break;
		case 1: // ADC
			break;
		case 2: // UART
			if(menu.pos[1] == 0)
			{
				if(uart0.status)
					UART0_sendString("<F0RRo>\r\n");
			}
			else if(menu.pos[1] == 1)
				UART0_setUp();
			else if(menu.pos[1] == 2)
				UART0_setDown();
			break;
		}
	}
	showMenu();
}

void SW1_handler(uint8_t st)
{
	if(st) // presionado
	{
		sw1_timerId = startTimer(600, enterMenu);
	}
	else   // soltado
	{
		if(!isTimerEnd(sw1_timerId)) // si está en curso
		{
			stopTimer(&sw1_timerId);
			if(LCD.isInMenu)
			{
				if(menu.level == 0)
				{
					menu.pos[0]++;
					menu.pos[0] %= 3;
				}
				else if(menu.level == 1)
				{
					switch(menu.pos[0])
					{
					case 0: // SALIDAS DIGITALES
						menu.pos[1]++;
						menu.pos[1] %= 4;
						break;
					case 1: // ADC
						break;
					case 2: // UART
						menu.pos[1]++;
						menu.pos[1] %= 3;
						break;
					}
				}
			}
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
		LCD_stopScroll();
		if(!LCD.isInClock)
			LCD_clear();
		if(LCD.isInMenu)
		{
			if(menu.level>0)
			{
				if(menu.pos[0] == 1) // adc
					stopADC();

				menu.pos[menu.level] = 0;
				menu.level--;
				showMenu();
			}
			else
			{
				LCD.isInClock = TRUE;
				restoreScreen();
			}
		}
		else if(!LCD.isInClock)
		{
			LCD.isInClock = TRUE;
			restoreScreen();
		}
	}
}

void SW3_handler(uint8_t st)
{
	if(st)
	{
		LCD_stopScroll();
		if(LCD.isInClock || LCD.isInMenu)
		{
			LCD.isOn = FALSE;
			LCD.isInClock = FALSE;
			LCD.isInMenu = FALSE;
			if(menu.level>0 && menu.pos[0] == 1)
				stopADC();
			menu.level = 0;
			LCD_clear();
		}
		else
		{
			LCD.isOn = TRUE;
			LCD.isInClock = TRUE;
			LCD.isInMenu = FALSE;
			LCD_displayClock();
		}
	}
}
/*
void SW3_handler(uint8_t st)
{
	if(st)
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
*/
void SW4_handler(uint8_t st) {}

#if _5_ENTRADAS
void SW5_handler(uint8_t st) {}
#endif

void tramaRecibida(char *msg)
{
	switch(msg[0])
	{
	// answer obtained
	case 'A':
		// time/date message
		if(msg[1] == 'T')
		{
			// check 2 bytes year + 1 month + 1 day + 1 hour + 1 min + 1 sec = 7 bytes
			if(strlen(msg+2) == 7)
				RTC_setTime_fromString(msg+2);
		}
	}

	// test de comunicacion
	if(!strncmp(msg, "PUT0", 4))
		UART0_sendString("<F0RRo>\r\n");
}
