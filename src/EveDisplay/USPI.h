﻿//------------------------------------------------------------------------------
// Реализация USART_1 в режиме SPI Master
// SPI mode 0 (CPOL & CPHA = 0), MSB First
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __USART_SPI_H
#define __USART_SPI_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//#include "Hardware.h"

//class CSpi;
//class CGpio;

#include "Platform.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class USPI
{
protected:
public:
    inline static unsigned char Send(unsigned char);       // Обмен данными
    static inline void Write(unsigned char);		// Запись байта данных
    static inline unsigned char Read(void);		// Чтение байта данных

    static inline void WaitingReceive(void);               // [##]

    static void Initialize(void);                          // Начальная инициализация

    static  void Write(void *,unsigned short);          // Запись блока данных [##]
//    static  void Write(void  *,unsigned short);  // Запись блока данных [##]
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//************************** INLINE - ФУНКЦИИ **********************************
//------------------------------------------------------------------------------
// ВНИМАНИЕ - Переделать на режим с использованием буферизации модуля USART
unsigned char USPI::Send(unsigned char Value)
{
//    /*                                               // НЕ РАБОТАЕТ
//     UDR1 = Value;                                   // Put data into buffer, sends the data
//     while ( !( UCSR1A & (1<<UDRE1)) );              // Wait for empty transmit buffer
//     return UDR1;                                    // Get and return received data from buffer
//    */
//
//    while ( !(UCSR1A & (1 << UDRE1)) );            // Wait for empty transmit buffer
//    UDR1 = Value;                                  // Put data into buffer, sends the data
//    while ( !(UCSR1A & (1 << RXC1)) );             // Wait for data to be received
//    return UDR1;                                   // Get and return received data from buffer
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void USPI::Write(unsigned char Value)
{
    Send(Value);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned char USPI::Read(void)
{
    return Send(0x00);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void USPI::WaitingReceive(void)                  // НЕ РАБОТАЕТ
{
// while ( !(UCSR1A & (1<<TXC1)) );              // Wait for data to be received
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif //__USART_SPI_H
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
