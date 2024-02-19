
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
void LCD_output(unsigned int);

//variable initialization
unsigned int num1, num2, sum,m,m;
unsigned char equal, plus,j,k[10],n;

void main() {
    LCD_PIC_init();
    num1 = 523;
    num2 = 238;
    sum = num1 + num2;
    equal = '=';
    plus = '+';
    LCD_command(0x80);
    LCD_output(num1);
    LCD_data(plus);
    LCD_output(num2);
    LCD_data(equal);
    LCD_output(sum);
    while (1);

}

void LCD_PIC_init() {
    //PIC initialization
    TRISC = 0x00; // setting port C as output //RS,Enable pin 
    TRISD = 0x00; // setting port D as output// data pins
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


void LCD_output(unsigned int i) {
    unsigned char s,j=1;
    m=i;
    while (i != 0) {
        s = m - ((m / 10)*10);
        k[j] = s;
        j++;
        m =m/10;
    }
    k[j] = '\0';
    j =j-1;
    while(j!=0)
    {
        n=0x30 + k[j];
        LCD_data(n);
        j--;
    }
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