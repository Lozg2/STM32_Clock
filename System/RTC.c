#include "stm32f10x.h"                  // Device header
#include "time.h"

uint16_t RTC_Time[6] = {NULL};
uint16_t Clock_Time[6] = {NULL};

void RTC_SetTime(void);

void RTC_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);

	PWR_BackupAccessCmd(ENABLE);
	
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	
	RTC_SetPrescaler(32768 - 1);
	RTC_WaitForLastTask();
	
	RTC_SetTime();
}

void RTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = RTC_Time[0] - 1900;
	time_date.tm_mon = RTC_Time[1] - 1;
	time_date.tm_mday = RTC_Time[2];
	time_date.tm_hour = RTC_Time[3];
	time_date.tm_min = RTC_Time[4];
	time_date.tm_sec = RTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();

}

void RTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	
	time_date = *localtime(&time_cnt);
	
	RTC_Time[0] = time_date.tm_year + 1900;
	RTC_Time[1] = time_date.tm_mon + 1;
	RTC_Time[2] = time_date.tm_mday;
	RTC_Time[3] = time_date.tm_hour;
	RTC_Time[4] = time_date.tm_min;
	RTC_Time[5] = time_date.tm_sec;
	
	
}	

void RTC_ReadSet(uint16_t SetClock)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = SetClock + 8 * 60 * 60;
	
	time_date = *localtime(&time_cnt);
	
	Clock_Time[0] = time_date.tm_year + 1900;
	Clock_Time[1] = time_date.tm_mon + 1;
	Clock_Time[2] = time_date.tm_mday;
	Clock_Time[3] = time_date.tm_hour;
	Clock_Time[4] = time_date.tm_min;
	Clock_Time[5] = time_date.tm_sec;
	
	
}

