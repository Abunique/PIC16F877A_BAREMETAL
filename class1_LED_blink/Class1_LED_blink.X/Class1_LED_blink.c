
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

/*LED BLINKING 
 LED is connected to the RC3 pin of PIC16F877A
 * TRIS bit configuration(0-output/1-input)
 * PORT configuration(1-high,0-low)
 */

#include <xc.h>

//Setting clock frequency
#define _XTAL_FREQ 6000000

void main()
{
    //configuring port C register
    TRISC = 0xF7; //1111 0111 Setting RC3 pin as output and all other pins as input
    
    while(1){
        //set RC3 pin HIGH
        PORTC=0x08; //0000 1000 output 5v in PC3 pin for LED ON state
        __delay_ms(1000); // LED ON for 1 second
        PORTC=0x00;// Turn OFF led 
        __delay_ms(1000);// LED OFF for 1 second
        
    }
}


