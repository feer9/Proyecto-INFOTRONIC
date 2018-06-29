#ifndef _TIMER_H
#define _TIMER_H

#include <RegsLPC1769.h>

#define N_TIMERS 8
#define TIMER_FINISHED 2
#define IS_TIMER_END 3

int32_t timer(int8_t n, uint8_t action, uint32_t time);
int8_t nextTimer(uint32_t* timerCount, uint8_t* timerState, uint8_t size);
void timerEnded(void);


/** funciones de usuario */
void timerEnd_Handler(uint8_t);
uint8_t isTimerEnd(uint8_t);
void stopTimer(uint8_t);
void startTimer(uint8_t n, uint32_t time);

#endif // _TIMER_H