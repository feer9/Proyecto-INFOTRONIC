#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/PLL.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/LCD.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"


void D_IN_init		( void );
void ExtInt_init	( void );
void SysTick_init	( uint32_t us );


void kit_init ( void )
{
	PLL_init();
	power_init();
	D_IN_init();
	TIMER0_init(1000);
	SysTick_init(2500);
	RTC_init();
	ExtInt_init();
	LCD_init(LCD_INTERNAL_RESET_DISABLED);
	UART0_init(U0_INIT_STATUS);
	ADC_init();
}