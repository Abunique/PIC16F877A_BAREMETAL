
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

// defining clock frequency for PIC controller 
//Same to be used in Proteus stimulation
#define _XTAL_FREQ 6000000

//function declaration
void LCD_PIC_init();
void LCD_command(unsigned char);
void LCD_data(unsigned char);
void LCD_output(float);
void keyscane();
void batt_msg();

//Variable declaration
unsigned char array[16] = {"BATTVOLT:    V"}, value;
unsigned char BATT_MSG1[16] = {"BATTERY LOW   "}, BATT_MSG2[16] = {"BATTERY NORMAL"}, BATT_MSG3[16] = {"BATTERY HIGH  "};
float BATT_VAL;

void main() {
    LCD_PIC_init();
    while (1) {
        keyscane();
    }
}

void keyscane() {
    value = PORTB & 0xF0;
    switch (value) {
        case 0xE0:
            LCD_command(0x80);
            for (unsigned char x = 0; x < 9; x++) {
                LCD_data(array[x]);
            }
            BATT_VAL = 15.0;
            LCD_output(BATT_VAL);
            LCD_data(array[13]);
            batt_msg();
            break;

        case 0xD0:
            BATT_VAL += 0.1;
            if (BATT_VAL > 22.5)BATT_VAL = 22.5;
            LCD_output(BATT_VAL);
            batt_msg();
            break;

        case 0xB0:
            BATT_VAL -= 0.1;
            if (BATT_VAL < 15.0)BATT_VAL = 15.0;
            LCD_output(BATT_VAL);
            batt_msg();
            break;

        case 0x70:
            BATT_VAL = 17.7;
            LCD_output(BATT_VAL);
            LCD_command(0xC0);
            batt_msg();
            break;
    }
}

void batt_msg() {
    if (BATT_VAL >= 15.0 && BATT_VAL < 17.5) {
        LCD_command(0xC0);
        for (unsigned char x = 0; x < 15; x++) {
            LCD_data(BATT_MSG1[x]);
        }
    } else if (BATT_VAL > 17.6 && BATT_VAL < 20.5) {
        LCD_command(0xC0);
        for (unsigned char x = 0; x < 15; x++) {
            LCD_data(BATT_MSG2[x]);
        }
    } else if (BATT_VAL > 20.6 && BATT_VAL <= 22.5) {
        LCD_command(0xC0);
        for (unsigned char x = 0; x < 15; x++) {
            LCD_data(BATT_MSG3[x]);
        }
    }
}

unsigned char d2, d1, d0;
void LCD_output(float i) {
    d0 = (i - (int) i)*10;
    d2 = (i / 10);
    d1 = (i - (d2 * 10));

    LCD_command(0x89);
    LCD_data(0x30 + d2);
    LCD_command(0x8A);
    LCD_data(0x30 + d1);
    LCD_command(0x8B);
    LCD_data('.');
    LCD_command(0x8C);
    LCD_data(0x30 + d0);
}

void LCD_PIC_init() {
    //PIC initialization
    TRISB = 0xF0; // setting port b RB4-RB7 as input for switches
    TRISC = 0x00; // setting port C as output //RS,Enable pin 
    TRISD = 0x00; // setting port D as output// data pins
    //OPTION_REG &= 0x7F; // using internal pull up register for port B
    //LCD initialization
    LCD_command(0x30);
    __delay_ms(100);
    LCD_command(0x30);
    __delay_ms(100);
    LCD_command(0x30);
    __delay_ms(100);
    LCD_command(0x38); // function set (number of lines and 5*7 matrix)
    __delay_ms(100);
    LCD_command(0x08); //display OFF
    __delay_ms(100);
    LCD_command(0x01); //clear display
    __delay_ms(100);
    LCD_command(0x06); //entry mode
    __delay_ms(100);
    LCD_command(0x0E); //display ON,cursor display ON
    __delay_ms(100);
}
//RS pin - LOW (for sending command)-connected to RC3)
//Enable pin to go high to low(connected to RC0)

void LCD_command(unsigned char hex) {
    PORTC &= 0xF7; //1111 0111(mask bit for AND)clearing RC3
    PORTD = hex;
    PORTC |= 0x01; //0000 0001(mask bit for OR)setting RC0
    PORTC &= ~0x01; //1111 1110(mask bit for AND)clearing RC0
    __delay_ms(100);
}

//RS pin - HIGH (for sending data)-connected to RC3)
//Enable pin to go high to low(connected to RC0)

void LCD_data(unsigned char hex) {
    PORTC |= 0x08; //0000 1000(mask bit for OR)setting RC3
    PORTD = hex;
    PORTC |= 0x01; //0000 0001(mask bit for OR)setting RC0
    PORTC &= ~0x01; //1111 1110(mask bit for AND)clearing RC0
    __delay_ms(100);
}