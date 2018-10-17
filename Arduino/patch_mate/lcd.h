#pragma once

#include <LiquidCrystal.h>

enum {
    LCD_RS = 2,
    LCD_E = 3,
    LCD_DB4 = 8,
    LCD_DB5 = 7,
    LCD_DB6 = 6,
    LCD_DB7 = 5,
};

static LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
static char lcd_buf[2][16];

inline void
lcd_buf_clear()
{
    memset(lcd_buf[0], ' ', 16);
    memset(lcd_buf[1], ' ', 16);
}

inline void
lcd_setup()
{
    lcd.begin(16, 2);
}

inline void
lcd_update(uint8_t col, uint8_t row, uint8_t size)
{
    lcd.setCursor(col, row);
    lcd.write(lcd_buf[row] + col, size);
}

inline void
lcd_update(uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
    lcd.write(lcd_buf[row][col]);
}

inline void
lcd_update()
{
    lcd_update(0, 0, 16);
    lcd_update(0, 1, 16);
}

inline void
lcd_write(uint8_t col, uint8_t row, uint8_t c)
{
    lcd.setCursor(col, row);
    lcd.write(c);
}

