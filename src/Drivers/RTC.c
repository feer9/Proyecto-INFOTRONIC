#include "RTC.h"
#include "LCD.h"



void RTC_init()
{
	PCONP |= PCONP_RTC;				// power control periferic rtc
	LPC_RTC->RTC_AUXEN _SET_BIT(4);	// the RTC Oscillator Fail detect interrupt is enabled

	if(LPC_RTC->RTC_AUX & _BIT(4))	// RTC Oscillator Fail detect flag.
	{
//		CCR->CLKEN  = 0 : The time counters are disabled so that they may be initialized.
//		CCR->CTCRST = 1 : The elements in the internal oscillator divider are reset
//		CCR->CCALEN = 1 : The calibration counter is disabled and reset to zero
		LPC_RTC->CCR = 0x12;

		LPC_RTC->CALIBRATION = 86400 | (0x01 << 17); 	// adelantaba ~1seg / dia

		RTC_resetTime();

		LPC_RTC->RTC_AUX _SET_BIT(4);	// clear flag

//		CCR->CLKEN  = 1: The time counters are enabled
//		CCR->CTCRST = 0: Remain reset until CCR[1] is changed to zero -> lo pongo a 0
//		CCR->CCALEN = 0 : The calibration counter is enabled and counting
		LPC_RTC->CCR = 0x01;
	}

	LPC_RTC->CIIR = 1UL;	// interrupcion cada: bit0->seg bit1->min bit2->hora...
	LPC_RTC->AMR = 0xFFUL;	// when 'n' bit is 1, the value is not compared for the alarm
	ICPR0 = NVIC_RTC;		// limpio interrupcion
	ISER0 = NVIC_RTC;		// habilito interrupcion en el NVIC
}

void RTC_IRQHandler(void)
{
	// RTCCIF (counter increment interrupt)
	if(LPC_RTC->ILR & _BIT(0))
	{
		LPC_RTC->ILR = _BIT(0);
		if(LCD_isInClock() && LCD_isOn())
			LCD_updateClock();
	}

	// RTCALF (alarma)
	if(LPC_RTC->ILR & _BIT(1))
	{
		LPC_RTC->ILR = _BIT(1);
		LPC_RTC->AMR = 0xFFUL; // desactivo alarma
	//	uint32_t dummy = PCON;
		PCON |= (1UL << 8);

		gpio_togglePin(LEDLPC_B);
	}
}

void RTC_resetTime()
{
	LPC_RTC->YEAR  = 2020;
	LPC_RTC->MONTH = 0;
	LPC_RTC->DOY   = 0;
	LPC_RTC->DOM   = 0;
	LPC_RTC->DOW   = 0;
	LPC_RTC->HOUR  = 0;
	LPC_RTC->MIN   = 0;
	LPC_RTC->SEC   = 0;

	// Guardo la fecha en que actualicé el rtc para ajustar la calibracion
	LPC_RTC->GPREG0 = 20200000;
	LPC_RTC->GPREG1 = 000000;
	LPC_RTC->GPREG2 = 0;
	LPC_RTC->GPREG3 = 0;
	LPC_RTC->GPREG4 = 0;
}

void RTC_setTime (rtc_t *rtc)
{
	LPC_RTC->SEC   = rtc->sec;
	LPC_RTC->MIN   = rtc->min;
	LPC_RTC->HOUR  = rtc->hour;
	LPC_RTC->DOM   = rtc->dayOfMonth;
	LPC_RTC->DOW   = rtc->dayOfWeek;
	LPC_RTC->DOY   = rtc->dayOfYear;
	LPC_RTC->MONTH = rtc->month;
	LPC_RTC->YEAR  = rtc->year;
}

void RTC_getTime(rtc_t *rtc)
{
	rtc->sec		= LPC_RTC->SEC;
	rtc->min		= LPC_RTC->MIN;
	rtc->hour		= LPC_RTC->HOUR;
	rtc->dayOfMonth	= LPC_RTC->DOM;
	rtc->dayOfWeek	= LPC_RTC->DOW;
	rtc->dayOfYear	= LPC_RTC->DOY;
	rtc->month		= LPC_RTC->MONTH;
	rtc->year		= LPC_RTC->YEAR;
}

void RTC_setTime_fromString(char *s)
{
	rtc_t rtc;
	rtc.year       = (uint32_t) (s[0] << 8) | s[1];
	rtc.month      = s[2];
	rtc.dayOfMonth = s[3];
	rtc.hour       = s[4];
	rtc.min        = s[5];
	rtc.sec        = s[6];

	rtc.dayOfWeek = 0;
	rtc.dayOfYear = 0;

	RTC_setTime(&rtc);
}

void RTC_setGPREG_fromTime ()
{
	uint32_t tmp;

	tmp = 10000 * LPC_RTC->YEAR + 100 * LPC_RTC->MONTH + LPC_RTC->DOM;
	LPC_RTC->GPREG0 = tmp;

	tmp = 10000 * LPC_RTC->HOUR + 100 * LPC_RTC->MIN   + LPC_RTC->SEC;
	LPC_RTC->GPREG1 = tmp;

	LPC_RTC->GPREG2 = 0; // starts count
}

void RTC_setAlarmInMinutes(uint32_t minutes)
{
	LPC_RTC->ALSEC = LPC_RTC->SEC;
	LPC_RTC->ALMIN = (LPC_RTC->MIN + minutes) % 60;

	// activo alarma por match en minutos y segundos
	LPC_RTC->AMR = 0xFFUL & ~0x03UL;
}

void RTC_setAlarmInSeconds(uint32_t seconds)
{
	LPC_RTC->ALSEC = (LPC_RTC->SEC + seconds) % 60;

	// activo alarma por match en segundos
	LPC_RTC->AMR = 0xFFUL & ~0x01UL;
}
