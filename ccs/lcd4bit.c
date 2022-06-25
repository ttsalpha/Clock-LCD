#include <stddef.h>

#define LCD_RS PIN_D6
#define LCD_EN PIN_D5

#define LCD_D4 PIN_D4
#define LCD_D5 PIN_C7
#define LCD_D6 PIN_C6
#define LCD_D7 PIN_C5

// khai bao misc
#define Line_1 0x80
#define Line_2 0xC0
#define Clear_Scr 0x01

// D/n Cong
#use standard_io(C)
#use standard_io(D)

// khoi tao LCD
void LCD_Init(void)
{
   LCD_SetData(0x00);
   delay_ms(200);
   output_low(LCD_RS);
   LCD_SetData(0x03);
   LCD_PulseEnable();
   LCD_PulseEnable();
   LCD_PulseEnable();
   LCD_SetData(0x02);
   LCD_PulseEnable();
   LCD_PutCmd(0x2C);
   LCD_PutCmd(0x0C);
   LCD_PutCmd(0x06);
   LCD_PutCmd(0x01);
}

void LCD_SetPosition(unsigned int cX)
{
   LCD_SetData(swap(cX) | 0x08);
   LCD_PulseEnable();
   LCD_SetData(swap(cX));
   LCD_PulseEnable();
}

void LCD_PutChar(unsigned int cX)
{
   output_high(LCD_RS);
   LCD_PutCmd(cX);
   output_low(LCD_RS);
}

void LCD_PutCmd(unsigned int cX)
{
   // duong truyen 4 bit nen se chia 1 byte thanh 2 phan nibble
   LCD_SetData(swap(cX)); // gui high nibble (4 bit cao)
   LCD_PulseEnable();
   LCD_SetData(swap(cX)); // gui low nibble (4 bit thap)
   LCD_PulseEnable();
}
void LCD_PulseEnable(void)
{
   output_high(LCD_EN);
   delay_us(3);
   output_low(LCD_EN);
   delay_ms(3);
}

void LCD_SetData(unsigned int cX)
{
   output_bit(LCD_D4, cX & 0x01);
   output_bit(LCD_D5, cX & 0x02);
   output_bit(LCD_D6, cX & 0x04);
   output_bit(LCD_D7, cX & 0x08);
}
