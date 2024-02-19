
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

void receiver_init(void);

unsigned char val;

void main() {
    receiver_init();
    while (1) {
        if (PIR1 & 0x20) // bit 5 RCIF: USART Receive Interrupt Flag bit (1 - receive done)
        {
            val = RCREG;
            switch (val) {
                case 'A':
                    PORTD = 0x02; //rd1 1 rd4 0
                    break;
                case 'B':
                    PORTD = 0x10; //rd1 0 rd4 1
                    break;
                case 'C':
                    PORTD = 0x12; //rd1 1 rd4 1
                    break;
                case 'D':
                    PORTD = 0x00; //rd1 0 rd4 0
                    break;


            }
        }
    }
}

void receiver_init(void){
    TRISD =0x00;//setting all pins as output
    PORTD = 0x00;//for switch case calculation clearing all bit.
    TRISC= 0xC0;//setting TX (rc7)and RX(rc6) pins for enabling UART communication
    RCSTA = 0x90;//1001 0000 
    //bit 7 -  SPEN: Serial Port Enable bit , bit 4  CREN: Continuous Receive Enable bit(1)
    TXSTA = 0x20;//0010 0000 bit 5  TXEN: Transmit Enable bit(1)
    SPBRG = 0x09;//00001001 from baudrate calculation 
    //(Asynchronous) desired Baud Rate(9600) = FOSC(6000000)/(64 (X(value to be given to reg) + 1))
}