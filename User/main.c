#include "stm32f10x.h"                  // Device header
#include "RTC.h"
#include "Serial.h"
#include "time.h"

time_t clock_cnt;

int main(void)
{	
	
	RTC_Init();
	Serial_Init();
	static uint8_t SET = 0;
	
	while (1)
	{
		RTC_ReadTime();
		
		if (Serial_RxFlag == 1)						      //SET_TIME
		{
			RTC_Time[0] = ((uint16_t) Serial_RxPacket[1]-'0')*1000 + ((uint16_t) Serial_RxPacket[2]-'0')*100 + ((uint16_t) Serial_RxPacket[3]-'0')*10 + ((uint16_t) Serial_RxPacket[4]-'0');
			RTC_Time[1] = ((uint16_t) Serial_RxPacket[6]-'0')*10 + ((uint16_t) Serial_RxPacket[7]-'0');
			RTC_Time[2] = ((uint16_t) Serial_RxPacket[9]-'0')*10 + ((uint16_t) Serial_RxPacket[10]-'0');
			RTC_Time[3] = ((uint16_t) Serial_RxPacket[12]-'0')*10 + ((uint16_t) Serial_RxPacket[13]-'0');
			RTC_Time[4] = ((uint16_t) Serial_RxPacket[15]-'0')*10 + ((uint16_t) Serial_RxPacket[16]-'0');
			RTC_Time[5] = ((uint16_t) Serial_RxPacket[18]-'0')*10 + ((uint16_t) Serial_RxPacket[19]-'0');
			
			RTC_Init();			
			Serial_SendPacket();
			Serial_RxFlag = 0;
		}
		
		else if (Serial_RxFlag == 2)						//SET_CLOCK
		{
			struct tm clock_data;
				clock_data.tm_year = RTC_Time[0] - 1900;
				clock_data.tm_mon = RTC_Time[1] - 1;
				clock_data.tm_mday = RTC_Time[2];
				clock_data.tm_hour = ((uint16_t) Serial_RxPacket[1]-'0')*10 + ((uint16_t) Serial_RxPacket[2]-'0');
				clock_data.tm_min = ((uint16_t) Serial_RxPacket[4]-'0')*10 + ((uint16_t) Serial_RxPacket[5]-'0');
				clock_data.tm_sec = ((uint16_t) Serial_RxPacket[7]-'0')*10 + ((uint16_t) Serial_RxPacket[8]-'0');
				clock_cnt = mktime(&clock_data) - 8 * 60 * 60;	//Get CLock_CNT
			
			RTC_Init();		
			Serial_SendString("\r\nALTER:");
			Serial_SendNumber(clock_data.tm_hour,2);
			Serial_SendString(":");
			Serial_SendNumber(clock_data.tm_min,2);
			Serial_SendString(":");
			Serial_SendNumber(clock_data.tm_sec,2);
			Serial_SendString(".\r\nSet Over");
			
			Serial_RxFlag = 0;
		}
		else if (Serial_RxFlag == 3)						//GET_TIME
		{
			RTC_ReadTime();
			for (int i = 0; i < 6; i++)
			{
        Serial_SendTime[i] = RTC_Time[i];
			}
			Serial_SendPacket();
			Serial_RxFlag = 0;
		}
		
		else if (Serial_RxFlag == 4)						//GET_CLOCK
		{
			RTC_ReadSet(clock_cnt);	
			Serial_SendString("\r\nALTER:");
			Serial_SendNumber(Clock_Time[3],2);
			Serial_SendString(":");
			Serial_SendNumber(Clock_Time[4] ,2);
			Serial_SendString(":");
			Serial_SendNumber( Clock_Time[5],2);
			Serial_SendString(".\r\nGet Over");
			
			Serial_RxFlag = 0;
		}
		
		/*CLOCK Working: LED PA2*/
		if(RTC_GetCounter()==clock_cnt)				//ON
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			if(SET == 0)
			{
				Serial_SendString("\r\nClock ON");
				SET = 1;
			}

		}
		if(RTC_GetCounter()==clock_cnt+30)    //OFF
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
			if(SET == 1)
			{
				Serial_SendString("\r\nClock OFF");
				SET = 0;
			}			
		}
	}
}	

