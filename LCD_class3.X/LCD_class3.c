
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


#define _XTAL_FREQ 6000000

//function declaration
void init();
void LCD_data(unsigned char);
void LCD_command(unsigned char);

//variable declaration
unsigned char array[15]={"Hello"};

void main()
{
    init();
    LCD_command(0x80);// address of the 1st line 1st position in display.
    for(int i=0;i<=4;i++){
        LCD_data(array[i]);
        __delay_ms(50);
    }
          
    while(1);
}

void init()
{
    TRISC = 0x00;//for initializing RC0,RC3 as output.
    TRISD = 0x00;//for initializing all pins of port D as output connected to data pins(d0-d7)LCD.
    LCD_command(0x30); // initializing LCD 
     __delay_ms(100);
     LCD_command(0x30);
     __delay_ms(100);
     LCD_command(0x30);
     __delay_ms(100);
     LCD_command(0x38);//0011 1000 D3=N- no of lines(for 2 lines - 1),D2- 5x7 dot matrix-0
      // LCD_command(0x08);//0000 1000 D3=N- no of lines(for 2 lines - 1),D2- 5x7 dot matrix-0 -doubt
         //  LCD_command(0x18);//0001 1000 D4=DL(8bit,4bit)D3=N- no of lines(for 2 lines - 1),D2- 5x7 dot matrix-0 -doubt
     __delay_ms(100);
  //LCD_command(0x06);// 0000 0110 D3= s/c(1-shift entire display D2=R/L 1-right)
  LCD_command(0x08);//Display OFF
     __delay_ms(100);  
  LCD_command(0x01);//display_clear
     __delay_ms(100);
     LCD_command(0x06);//Entry mode set
     __delay_ms(100);
      LCD_command(0x0E);// 0000 0100 D3= s/c(1-shift entire display D2=R/L 1-right)//try
     __delay_ms(100);
     
}
void LCD_command(unsigned char i)
{
    RC3 = 0;// 1111 0111 RC3 is given 0 //Register Select pin is given input 0 to take commands
    PORTD = i; //command given by the user is given to port D
    RC0 = 1;// Enable pin connected to RC0=1 
    RC0= 0;//Enable pin is made 0,RC0=0
    __delay_ms(100); //to allow commands to sync
}
void LCD_data(unsigned char i)
{
    RC3 = 1;// 0000 1000 RC3 is given 1 //Register Select pin is given input 1 to take Data 
    PORTD = i; //DATA given by the user is given to d0 to d7 pins of LCD
    RC0 = 1;// Enable pin connected to RC0=1 
    RC0 = 0;//Enable pin is made 0,RC0=0
     __delay_ms(100); //to allow data to sync
}