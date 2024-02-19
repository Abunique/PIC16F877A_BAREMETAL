
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

//defining clock frequency for the PIC controller
#define _XTAL_FREQ 6000000

//function declaration
void ADC_LCD_PIC_init(void);
void LCD_command(unsigned char);
void LCD_data(unsigned char);
void LCD_output(unsigned int);

//variable initialization
unsigned char L_value, H_value;
long value, voltage;
unsigned char num_arr[10]; // for storing LCD_output values

void main(void) {
    ADC_LCD_PIC_init();
    while (1) {
        ADCON0 |= 0x04; // starting ADC conversion
        while (ADCON0 & 0x04);// checking GO_done bit for 0 ,indication for ADC_over status
        L_value = ADRESL;//2 parts of the ADC conversion is stored in as right justified.all 8 bits are occupied
        H_value = ADRESH;//MSB 2 bits will be stored in ADRESH
        value = ((unsigned int) H_value << 8)+(unsigned int) L_value; // total ADC value is adjusted using shift operator and stored
        voltage = value * 48 / 1023; //calibration to user defined value.'48'- MAX value to be displayed.
       //sending data to the LCD
        LCD_command(0X80);// specifying the start position in LCD
        if (voltage == 0)LCD_data(0x30);
        else LCD_output(voltage);
        LCD_data(0x56); // ASCII value for V.
        LCD_data(0x20);//last to digits are cleared for avoiding extra numbers from previously displayed values
        LCD_data(0x20);//ASCII 0X20 indicates space.
        __delay_ms(1000);
    }

}

//function definitions

void ADC_LCD_PIC_init(void) {
    //PIC_LCD initialization
    TRISC = 0x00; // setting port C as output //RS,Enable pin 
    TRISD = 0x00; // setting port D as output// data pins
    //PIC_ADC initialization
    TRISA = 0x01; //0000 0001 setting RA0 as input - AN0 channel for ADC
    //bit 0(1) - ADC power ON
    //bit 3 to 5 (000)- selecting channel AN0
    //bit 6,7(10)- for choosing fosc/32
    ADCON0 = 0x81; //1000 0001 
    //bit 3to0(1110)- AN0 alone as analog input other channel as digital
    //bit 6(0)- for choosing fosc/32
    //bit 7(1)-1 = Right justified. Six (6) Most Significant bits of ADRESH are read as ?0?. 
    ADCON1 = 0x8E; //1000 1110
    //LCD initialization
    LCD_command(0x30);
    __delay_ms(50);
    LCD_command(0x30);
    __delay_ms(50);
    LCD_command(0x30);
    __delay_ms(50);
    LCD_command(0x38); // function set (number of lines and 5*7 matrix)
    while (RD7 == 1); //checking busy flag connected D7 bit for ready condition
    LCD_command(0x08); //display OFF
    while (RD7 == 1); //checking busy flag connected D7 bit for ready condition
    LCD_command(0x01); //clear display
    while (RD7 == 1); //checking busy flag connected D7 bit for ready condition
    LCD_command(0x06); //entry mode
    while (RD7 == 1); //checking busy flag connected D7 bit for ready condition
    LCD_command(0x0E); //display ON,cursor display ON
    while (RD7 == 1);
}

void LCD_output(unsigned int n) {
    unsigned char result, i = 0;
    while (n != 0) {
        result = n - ((n / 10)*10);
        num_arr[i] = result;
        i++;
        n /= 10;
    }

    num_arr[i] = '\0';

    i -= 1;
    for (int j = i; j >= 0; j--) {
        LCD_data(0x30 + num_arr[j]);
    }
}

//RS pin - LOW (for sending command)-connected to RC3)
//Enable pin to go high to low(connected to RC0)

void LCD_command(unsigned char hex) {
    PORTC &= 0xF7; //1111 0111(mask bit for AND)clearing RC3
    PORTD = hex;
    PORTC |= 0x01; //0000 0001(mask bit for OR)setting RC0
    PORTC &= ~0x01; //1111 1110(mask bit for AND)clearing RC0
    __delay_ms(50);
}

//RS pin - HIGH (for sending data)-connected to RC3)
//Enable pin to go high to low(connected to RC0)

void LCD_data(unsigned char hex) {
    PORTC |= 0x08; //0000 1000(mask bit for OR)setting RC3
    PORTD = hex;
    PORTC |= 0x01; //0000 0001(mask bit for OR)setting RC0
    PORTC &= ~0x01; //1111 1110(mask bit for AND)clearing RC0
    __delay_ms(50);
}