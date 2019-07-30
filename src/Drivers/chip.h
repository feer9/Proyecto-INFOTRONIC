#ifndef _CLOCK_H
#define _CLOCK_H

#include <lpc_types.h>

//////////////Registros del CLOCK y de sistema/////////////////
//0x400FC1A0UL: Registro de control de sistema y registro de status:
#define		DIR_SCS				( (__RW uint32_t *) 0x400FC1A0UL)
//0x400FC104UL: Registro de configuracion del clock:
#define		DIR_CCLKCFG			( (__RW uint32_t *) 0x400FC104UL)
//0x400FC10CUL: Registro de seleccion del clock:
#define		DIR_CLKSRCSEL		( (__RW uint32_t *) 0x400FC10CUL)
//0x400FC1C8UL: Clock Output Config register:
#define		DIR_CLKOUTCFG		( (__RW uint32_t *) 0x400FC1C8UL)
//0x400FC000UL: Flash access configuration:
#define		DIR_FLASHCFG		( (__RW uint32_t *) 0x400FC000UL)

/////////////////Registros de los PLL///////////////////////////
#define		PLL0				( (__RW uint32_t *) 0x400FC080UL)
//0x400FC080UL: Registro de control del PLL0:
#define		DIR_PLL0CON			( (__RW uint32_t *) 0x400FC080UL)
//0x400FC084UL: Registro de configuracion del PLL0:
#define		DIR_PLL0CFG			( (__RW uint32_t *) 0x400FC084UL)
//0x400FC088UL: Registro de estado del PLL0:
#define		DIR_PLL0STAT		( (__RW uint32_t *) 0x400FC088UL)
//0x400FC08CUL: Registro de control del PLL0:
#define		DIR_PLL0FEED		( (__RW uint32_t *) 0x400FC08CUL)
//0x400FC0A0UL: Registro de control del PLL1:
#define		DIR_PLL1CON			( (__RW uint32_t *) 0x400FC0A0UL)
//0x400FC0A4UL: Registro de configuracion del PLL1:
#define		DIR_PLL1CFG			( (__RW uint32_t *) 0x400FC0A4UL)
//0x400FC0A8UL: Registro de estado del PLL1:
#define		DIR_PLL1STAT		( (__RW uint32_t *) 0x400FC0A8UL)
//0x400FC0ACUL: Registro de control del PLL1:
#define		DIR_PLL1FEED		( (__RW uint32_t *) 0x400FC0ACUL)

//Registro de status y configuracion del sistema:
#define		SCS			DIR_SCS[0]
#define 	FLASHCFG	DIR_FLASHCFG[0]

//Registros de control del CLOCK:
#define		CCLKCFG		DIR_CCLKCFG[0]
#define		CLKSRCSEL	DIR_CLKSRCSEL[0]
#define		CLKOUTCFG	DIR_CLKOUTCFG[0]

//PLL0:
#define		PLL0CON		DIR_PLL0CON[0]
#define		PLL0CFG		DIR_PLL0CFG[0]
#define		PLL0STAT	DIR_PLL0STAT[0]
#define		PLL0FEED	DIR_PLL0FEED[0]

//PLL1:
#define		PLL1CON		DIR_PLL1CON[0]
#define		PLL1CFG		DIR_PLL1CFG[0]
#define		PLL1STAT	DIR_PLL1STAT[0]
#define		PLL1FEED	DIR_PLL1FEED[0]

//Valores para configuracion del PLL:
#define CLOCK_SETUP_Value 		1
#define SCS_Value				0x00000020
#define CLKSRCSEL_Value         0x00000001
#define PLL0_SETUP_Value        1
#define PLL0CFG_Value           0x00050063
#define PLL1_SETUP_Value        1
#define PLL1CFG_Value           0x00000023
#define CCLKCFG_Value           0x00000003
#define USBCLKCFG_Value         0x00000000
#define PCLKSEL0_Value          0x00000000
#define PCLKSEL1_Value          0x00000000
#define PCONP_Value             0x042887DE
#define CLKOUTCFG_Value         0x00000000
#define FLASHCFG_Value			0x00004000


#define SYSCTL_IRC_FREQ (4000000)


#define PLL0STS_ENABLED   (1 << 24)	/*!< PLL0 enable flag */
#define PLL0STS_CONNECTED (1 << 25)	/*!< PLL0 connect flag */
#define PLL0STS_LOCKED    (1 << 26)	/*!< PLL0 connect flag */
#define PLL1STS_ENABLED   (1 << 8)	/*!< PLL1 enable flag */
#define PLL1STS_CONNECTED (1 << 9)	/*!< PLL1 connect flag */
#define PLL1STS_LOCKED    (1 << 10)	/*!< PLL1 connect flag */

/* Selectable PLLs */
typedef enum {
	SYSCTL_MAIN_PLL,		/*!< Main PLL (PLL0) */
	SYSCTL_USB_PLL,			/*!< USB PLL (PLL1) */
} SYSCTL_PLL_T;

typedef enum CCLKSRC {
	CCLKSRC_SYSCLK,			/*!< Select Sysclk as the input to the CPU clock divider. */
	CCLKSRC_MAINPLL			/*!< Select the output of the Main PLL as the input to the CPU clock divider. */
} CCLKSRC_T;


/* PLL source clocks */
typedef enum PLLCLKSRC {
	PLLCLKSRC_IRC,			/*!< PLL is sourced from the internal oscillator (IRC) */
	PLLCLKSRC_MAINOSC,		/*!< PLL is sourced from the main oscillator */
	PLLCLKSRC_RTC,			/*!< PLL is sourced from the RTC oscillator */
	PLLCLKSRC_RESERVED
} PLLCLKSRC_T;


//!< ///////////////////   PCLKSEL   //////////////////////////
/**
 * Peripheral clock divider rates used with the
 * PCLKSEL_T clock types (devices only)
 */
typedef enum {
	PCLKDIV_4,			/*!< Divider by 4 */
	PCLKDIV_1,			/*!< Divider by 1 */
	PCLKDIV_2,			/*!< Divider by 2 */
	PCLKDIV_8,			/*!< Divider by 8, not for use with CAN */
	PCLKDIV_6_CCAN = PCLKDIV_8	/*!< Divider by 6, CAN only */
} PCLKDIV_T;

/**
 * Peripheral clock selection for LPC175x/6x
 * This is a list of clocks that can be divided on the 175x/6x
 */
typedef enum {
	PCLKSEL_WDT,		/*!< Watchdog divider */
	PCLKSEL_TIMER0,		/*!< Timer 0 divider */
	PCLKSEL_TIMER1,		/*!< Timer 1 divider */
	PCLKSEL_UART0,		/*!< UART 0 divider */
	PCLKSEL_UART1,		/*!< UART 1 divider */
	PCLKSEL_RSVD5,
	PCLKSEL_PWM1,		/*!< PWM 1 divider */
	PCLKSEL_I2C0,		/*!< I2C 0 divider */
	PCLKSEL_SPI,		/*!< SPI divider */
	PCLKSEL_RSVD9,
	PCLKSEL_SSP1,		/*!< SSP 1 divider */
	PCLKSEL_DAC,		/*!< DAC divider */
	PCLKSEL_ADC,		/*!< ADC divider */
	PCLKSEL_CAN1,		/*!< CAN 1 divider */
	PCLKSEL_CAN2,		/*!< CAN 2 divider */
	PCLKSEL_ACF,		/*!< ACF divider */
	PCLKSEL_QEI,		/*!< QEI divider */
	PCLKSEL_GPIOINT,	/*!< GPIOINT divider */
	PCLKSEL_PCB,		/*!< PCB divider */
	PCLKSEL_I2C1,		/*!< I2C 1 divider */
	PCLKSEL_RSVD20,
	PCLKSEL_SSP0,		/*!< SSP 0 divider */
	PCLKSEL_TIMER2,		/*!< Timer 2 divider */
	PCLKSEL_TIMER3,		/*!< Timer 3 divider */
	PCLKSEL_UART2,		/*!< UART 2 divider */
	PCLKSEL_UART3,		/*!< UART 3 divider */
	PCLKSEL_I2C2,		/*!< I2C 2 divider */
	PCLKSEL_I2S,		/*!< I2S divider */
	PCLKSEL_RSVD28,
	PCLKSEL_RIT,		/*!< Repetitive timer divider */
	PCLKSEL_SYSCON,		/*!< SYSCON divider */
	PCLKSEL_MCPWM		/*!< Motor control PWM divider */
} PCLKSEL_T;



//!< ///////////////////   PCONP   //////////////////////////

#define 	PCONP_TIMER0	(1UL << 1)
#define 	PCONP_TIMER1	(1UL << 2)
#define 	PCONP_UART0		(1UL << 3)
#define 	PCONP_UART1		(1UL << 4)

#define 	PCONP_PWM1		(1UL << 6)
#define 	PCONP_I2C0		(1UL << 7)
#define 	PCONP_SPI		(1UL << 8)
#define 	PCONP_RTC		(1UL << 9)
#define 	PCONP_SSP1		(1UL << 10)

#define 	PCONP_ADC		(1UL << 12)
#define 	PCONP_CAN1		(1UL << 13)
#define 	PCONP_CAN2		(1UL << 14)
#define 	PCONP_GPIO		(1UL << 15)
#define 	PCONP_RIT		(1UL << 16)
#define 	PCONP_MCPWM		(1UL << 17)
#define 	PCONP_QEI		(1UL << 18)
#define 	PCONP_I2C1		(1UL << 19)

#define 	PCONP_SSP0		(1UL << 21)
#define 	PCONP_TIM2		(1UL << 22)
#define 	PCONP_TIM3		(1UL << 23)
#define 	PCONP_UART2		(1UL << 24)
#define 	PCONP_UART3		(1UL << 25)
#define 	PCONP_I2C2		(1UL << 26)
#define 	PCONP_I2S		(1UL << 27)

#define 	PCONP_GPDMA		(1UL << 29)
#define 	PCONP_ENET		(1UL << 30)
#define 	PCONP_USB		(1UL << 31)



/** System oscillator rate
 * This value is defined externally to the chip layer and contains
 * the value in Hz for the external oscillator for the board. If using the
 * internal oscillator, this rate can be 0.
 */
extern const uint32_t OscRateIn;

/** RTC oscillator rate
 * This value is defined externally to the chip layer and contains
 * the value in Hz for the RTC oscillator for the board. This is
 * usually 32KHz (32768). If not using the RTC, this rate can be 0.
 */
extern const uint32_t RTCOscRateIn;

/* Current system clock rate, mainly used for sysTick */
extern uint32_t SystemCoreClock;



// Prototipos de función:
void      PLL_init  ( void );
void      power_init( void );
uint32_t  getPCLKDiv(PCLKSEL_T clk);
void      setPCLKDiv(PCLKSEL_T clk, PCLKDIV_T div);
CCLKSRC_T getCPUClockSource(void);
uint32_t  getSYSCLKRate(void);
uint32_t  getMainClockRate(void);
uint32_t  getMainPLLOutClockRate(void);


static inline uint32_t getCPUClockDiv(void)
{
	return (CCLKCFG & 0xFF) + 1;
}

static inline PLLCLKSRC_T getMainPLLSource(void)
{
	return (PLLCLKSRC_T) (CLKSRCSEL & 0x3);
}

static inline uint32_t getMainPLLInClockRate(void)
{
	return getSYSCLKRate();
}

static inline bool isMainPLLConnected(void)
{
	return (bool) ((PLL0STAT & PLL0STS_CONNECTED) != 0);
}

static inline bool isMainPLLEnabled(void)
{
	return (bool) ((PLL0STAT & PLL0STS_ENABLED) != 0);
}

static inline uint32_t getSystemClockRate(void)
{
	return getMainClockRate() / getCPUClockDiv();
}

static inline void SystemCoreClockUpdate(void)
{
	SystemCoreClock = getSystemClockRate();
}

#endif /* _CLOCK_H */