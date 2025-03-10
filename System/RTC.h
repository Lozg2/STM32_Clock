#ifndef __RTC_H
#define __RTC_H

extern uint16_t RTC_Time[];
extern uint16_t Clock_Time[];

void RTC_Init(void);
void RTC_SetTime(void);
void RTC_ReadTime(void);
void RTC_ReadSet(uint16_t SetClock);

#endif
