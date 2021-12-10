#pragma once

#include "config.h"

enum {
    LCD_LINES = 2,
    LCD_COLUMNS = 16,
};


#if defined(ARDUINO)
#if defined(ARDUINO_AVR_PATCHMATE_X)
static spi_oled lcd;
#else
enum {
    LCD_RS = 2,
    LCD_E = 3,

    LCD_DB4 = 4,
    LCD_DB5 = 5,
    LCD_DB6 = 6,
    LCD_DB7 = 7,
};

static LiquidCrystal lcd(LCD_RS, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
#endif
#else
static LiquidCrystal lcd;
#endif

static char lcd_buf[LCD_LINES][LCD_COLUMNS];

inline void
lcd_buf_clear()
{
    for (uint8_t i = 0; i < LCD_LINES; i++) {
        memset(lcd_buf[i], ' ', LCD_COLUMNS);
    }
}

inline void
lcd_buf_clear(uint8_t line)
{
    memset(lcd_buf[line], ' ', LCD_COLUMNS);
}

inline void
lcd_setup()
{
    lcd.begin(LCD_COLUMNS, LCD_LINES);
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
    for (uint8_t i = 0; i < LCD_LINES; i++) {
        lcd_update(0, i, LCD_COLUMNS);
    }
}

inline void
lcd_write(uint8_t col, uint8_t row, uint8_t c)
{
    lcd.setCursor(col, row);
    lcd.write(c);
}

