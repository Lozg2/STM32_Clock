#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint16_t Serial_SendTime[];
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);

void Serial_SendPacket(void);

#endif
