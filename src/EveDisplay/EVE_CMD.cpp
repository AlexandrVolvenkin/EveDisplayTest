﻿//------------------------------------------------------------------------------
// Command coprocessor layer for FTDI EVE2 (only FT81x chips)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "EVE_CMD.h"
#include "EVE_Resources.h"
#include "USPI.h"

#include "Platform.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//***************************** ДАННЫЕ *****************************************
//------------------------------------------------------------------------------
unsigned short EVE_CMD::P;
unsigned short EVE_CMD::CMD_Read;
unsigned short EVE_CMD::CMD_Write;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Таблица транскодирования Win-1251 в "EVE_CYR_FONT_TABLE"
unsigned char  CYR_Table[256] =
{
    0x00, '?', '?', '?',    '?', '?', '?', '?',     '?', '?', '?', '?',     '?', '?', '?', '?',    // 0
    '?', '?', '?', '?',    '?', '?', '?', '?',     '?', '?', '?', '?',     '?', '?', '?', '?',    // 1
    ' ', '!', '"', '#',    '?', '%', '?', '?',     '(', ')', '*', '+',     ',', '-', '.', '/',    // 2
    '0', '1', '2', '3',    '4', '5', '6', '7',     '8', '9', ':', ';',     '<', '=', '>', '?',    // 3

    '?', 'A', 'B', 'C',    'D', 'E', 'F', 'G',     'H', 'I', 'J', 'K',     'L', 'M', 'N', 'O',    // 4
    'P', 'Q', 'R', 'S',    'T', 'U', 'V', 'W',     'X', 'Y', 'Z', '?',     '?', '?', '^', '_',    // 5
    '?', 'a', 'b', 'c',    'd', 'e', 'f', 'g',     'h', 'i', 'j', 'k',     'l', 'm', 'n', 'o',    // 6
    'p', 'q', 'r', 's',    't', 'u', 'v', 'w',     'x', 'y', 'z', '?',     '?', '?', '?', '?',    // 7

    '?', '?', '?', '?',    '?', '?', '?', '?',     '?', '?', '?', '?',     '?', '?', '?', '?',    // 8
    '?', '?', '?', '?',    '?', '?', '?', '?',     '?', '?', '?', '?',     '?', '?', '?', '?',    // 9
    '?', '?', '?', '?',    '?', '?', '?', '?',     '?', '?', '?', '?',     '?', '?', '?', '?',    // A
    '?', '?', '?', '?',    '?', '?', '?', '?',     '?',0x40, '?', '?',     '?', '?', '?', '?',    // B

    0x41,0x01,0x42,0x03,  0x05,0x45,0x07,0x09,    0x0B,0x0D,0x4B,0x0F,    0x4D,0x48,0x4F,0x11,     // C
    0x50,0x43,0x54,0x7C,  0x13,0x58,0x15,0x17,    0x19,0x1B, '?', '?',     '?',0x1D,0x1F,0x26,     // D
    0x61,0x02,0x5B,0x04,  0x06,0x65,0x08,0x0A,    0x0C,0x0E,0x5C,0x10,    0x5D,0x60,0x6F,0x12,     // E
    0x70,0x63,0x7B,0x79,  0x14,0x78,0x16,0x18,    0x1A,0x1C,0x7D,0x7E,    0x7F,0x1E,0x24,0x27      // F
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//***************************** ФУНКЦИИ ****************************************
//------------------------------------------------------------------------------
// Начало транзакции записи по заданному адресу с подсчетом позиций
void EVE_CMD::WriteTransaction(unsigned long Address)
{
    Enable();

    USPI::Write(((Address >> 16L) & MEM_MASK) | MEM_WRITE);
    USPI::Write(HIGH(Address));
    USPI::Write( LOW(Address));

    P = 0;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Завершение транзакции
void EVE_CMD::EndTransaction(void)
{
    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Write32(unsigned long Value)
{
    USPI::Write( LOW(Value));
    USPI::Write(HIGH(Value));
    USPI::Write(Value >> 16L);
    USPI::Write(Value >> 24L);

    P++;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Начало DL-листа
void EVE_CMD::BeginDL(void)
{
    /*
     CMD_Read = EVE_CMD::Read16(REG_CMD_READ);

     EVE_CMD::WriteTransaction(EVE_HAL::RAM_CMD + CMD_Read);
    */

    CMD_Write = EVE_CMD::Read16(REG_CMD_WRITE);

    EVE_CMD::WriteTransaction(EVE_HAL::RAM_CMD + CMD_Write);

};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Конец DL-листа
void EVE_CMD::EndDL(void)
{
    Disable();
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запуск DL-листа на выполнение
void EVE_CMD::Execute(unsigned short Offset)
{
//EVE_HAL::Write32(REG_CMD_WRITE, CMD_Read + Offset + (EVE_CMD::P * 4));

    EVE_HAL::Write32(REG_CMD_WRITE, CMD_Write + Offset + (EVE_CMD::P * 4));
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись команды
void EVE_CMD::WriteCMD(unsigned char Cmd)
{
    USPI::Write(Cmd);
    USPI::Write(0xFF);
    USPI::Write(0xFF);
    USPI::Write(0xFF);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись 4-байтной величины
void EVE_CMD::Write32F(unsigned long Value)
{
    USPI::Write( LOW(Value));
    USPI::Write(HIGH(Value));
    USPI::Write(Value >> 16L);
    USPI::Write(Value >> 24L);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Запись 1-байтной величины
void EVE_CMD::Write08F(unsigned char Value)
{
    USPI::Write(Value);
    USPI::Write(0x00);
    USPI::Write(0x00);
    USPI::Write(0x00);
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Выравнивание блока данных на границу 4 байт
void EVE_CMD::Alignment(unsigned char Number)
{
    for(unsigned char i = 0; i < Number; i++)
    {
        USPI::Write(0x00);
    };
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Изменение оринтации экрана дисплея
void EVE_CMD::SetRotate(unsigned char Orientation)
{
    WriteCMD(LOW(CMD_SETROTATE));
    Write08F(Orientation);
    P += 2;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Установка основания для вывода чисел BIN/OCT/DEC/HEX = 2/8/10/16
void EVE_CMD::SetBase(unsigned char Base)
{
    WriteCMD(LOW(CMD_SETBASE));
    Write08F(Base);
    P += 2;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::RomFont(unsigned char Font,unsigned char RomSlot)
{
    WriteCMD(LOW(CMD_ROMFONT));
    Write08F(Font);
    Write08F(RomSlot);
    P += 3;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetFont(unsigned long Address,unsigned char Font,unsigned char FirstChar)
{
    WriteCMD(LOW(CMD_SETFONT2));
    Write08F(Font);
    Write32F(Address);
    Write08F(FirstChar);
    P += 4;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetBGColor(unsigned char Red,unsigned char Green,unsigned char Blue)
{
    WriteCMD(LOW(CMD_BGCOLOR));
    Write32F(COLOR_RGB(Red,Green,Blue));
    P += 2;

// Write32F(0x00FFFFFF & COLOR_RGB(Red,Green,Blue));      P += 2;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetBGColor(unsigned long Color)
{
    WriteCMD(LOW(CMD_BGCOLOR));
    Write32F(COLOR_RGB_NEW(Color));
    P += 2;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::SetFGColor(unsigned char Red,unsigned char Green,unsigned char Blue)
{
    WriteCMD(LOW(CMD_FGCOLOR));
    Write32F(COLOR_RGB(Red,Green,Blue));
    P += 2;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Вывод текстовой строки
 void EVE_CMD::String(char *Str)
{
    unsigned char Lenght = 0;
    char Byte;

    while((Byte = *Str++))
    {
        USPI::Write(Byte);
        Lenght++;
    };

    unsigned char Dop  = 4 - Lenght % 4;

    Lenght += Dop;

    do
    {
        USPI::Write(0x00);
    }
    while(--Dop);

    P += Lenght / 4;
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//// Вывод текстовой строки из Flash
// void EVE_CMD::String(char __farflash *Str)
//{
//    unsigned char Lenght = 0;
//    char Byte;
//
//    while((Byte = *Str++))
//    {
//        USPI::Write(Byte);
//        Lenght++;
//    };
//
//    unsigned char Dop  = 4 - Lenght % 4;
//
//    Lenght += Dop;
//
//    do
//    {
//        USPI::Write(0x00);
//    }
//    while(--Dop);
//
//    P += Lenght / 4;
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Вывод текстовой строки для кириллического шрифта
 void EVE_CMD::StringCYR(char *Str)
{
    unsigned char Lenght = 0;
    char Byte;

    while((Byte = *Str++))
    {
        unsigned char Character = CYR_Table[Byte];

        USPI::Write(Character);
        Lenght++;
    };

    unsigned char Dop  = 4 - Lenght % 4;

    Lenght += Dop;

    do
    {
        USPI::Write(0x00);
    }
    while(--Dop);

    P += Lenght / 4;
};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//// Вывод текстовой строки из Flash для кириллического шрифта
// void EVE_CMD::StringCYR(char __farflash *Str)
//{
//    unsigned char Lenght = 0;
//    char Byte;
//
//    while((Byte = *Str++))
//    {
//        unsigned char Character = CYR_Table[Byte];
//
//        USPI::Write(Character);
//        Lenght++;
//    };
//
//    unsigned char Dop  = 4 - Lenght % 4;
//
//    Lenght += Dop;
//
//    do
//    {
//        USPI::Write(0x00);
//    }
//    while(--Dop);
//
//    P += Lenght / 4;
//};
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
//// Вывод потока 4-байтовых значений
// void EVE_CMD::WriteStream(unsigned long  *Data,unsigned char Lenght)
//{
//    unsigned char  *Ptr = ((unsigned char  *)Data);
//    unsigned char    SizeBytes = 4 * Lenght;
//
//    for(unsigned char i = 0; i < SizeBytes; i++)
//    {
//        USPI::Write(*Ptr++);
//    };
//
//    P += Lenght;
//};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Text(unsigned short X,unsigned short Y,unsigned char Font,unsigned short Options)
{
    if(Options & OPT_PRESS)                // Отображение нажатия
    {
        Options &= ~OPT_PRESS;

        X -= 2;
        Y -= 2;
    };

    WriteCMD(LOW(CMD_TEXT));
    Write32F(POSXY(X,Y));
    Write32F(PACK(Options,Font));
    P += 3;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Number(unsigned short X,unsigned short Y,unsigned char Font,unsigned short Options,long Value)
{
    WriteCMD(LOW(CMD_NUMBER));
    Write32F(POSXY(X,Y));
    Write32F(PACK(Options,Font));
    Write32F(Value);
    P += 4;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Keys(unsigned short X,unsigned short Y,unsigned char W,unsigned char H,unsigned char Font,unsigned short Options)
{
    WriteCMD(LOW(CMD_KEYS));
    Write32F(POSXY(X,Y));
    Write32F(PACK(H,W));
    Write32F(PACK(Options,Font));
    P += 4;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Button(unsigned short X,unsigned short Y,unsigned char W,unsigned char H,unsigned char Font,unsigned short Options)
{
    if(Options & OPT_PRESS)                // Отображение нажатия
    {
        Options &= ~OPT_PRESS;

        X -= 2;
        Y -= 2;
    };

    WriteCMD(LOW(CMD_BUTTON));
    Write32F(POSXY(X,Y));
    Write32F(PACK(H,W));
    Write32F(PACK(Options,Font));
    P += 4;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Clock(unsigned short X,unsigned short Y,unsigned char Radius,unsigned short Options)
{
    WriteCMD(LOW(CMD_CLOCK));
    Write32F(POSXY(X,Y));
    Write32F(PACK(Options,Radius));
    P += 3;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Track(unsigned short X,unsigned short Y,unsigned short W,unsigned short H,unsigned char Tag)
{
    WriteCMD(LOW(CMD_TRACK));
    Write32F(POSXY(X,Y));
    Write32F(PACK(H,W));
    Write08F(Tag);
    P += 4;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EVE_CMD::Append(unsigned long Ptr,unsigned short Count)
{
    WriteCMD(LOW(CMD_APPEND));
    Write32F(Ptr);
    Write32F(4UL * Count);
    P += 3;
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*

    EVE_CMD::DL(CMD_CLOCK);                             // Часы по центру
    EVE_CMD::DL(PACK(195,255));                         // Y,X
    EVE_CMD::DL(PACK(OPT_NOBACK | OPT_NOSECS,50));      // Options = 0, R
    EVE_CMD::DL(PACK(RTC::GetMinutes(),RTC::GetHours()));
    EVE_CMD::DL(PACK(0,RTC::GetSeconds()));
*/
