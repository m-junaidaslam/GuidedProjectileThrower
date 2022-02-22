#include<p18f45k22.h>
#include "uart_45k22.h"
#include "LCD_4bit.h"
#include "GPS_DUALUART_Parsing.h"
#include "Stepper_Drive.h"
#include "ADXL335_ADC_45K22.h"
#include "TIMER_45k222.h"

#pragma config FOSC=INTIO67
#pragma config STVREN = OFF
#pragma config MCLRE = INTMCLR
#pragma config WDTEN = OFF

volatile unsigned BYTE byte1;       //Contains data coming from UART 1

void Init() {
    OSCCON = 0x76;      //For 16MHz from internal osscilator with FOSC = INTIO67
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 1;
    lcd_ini();
    clr_scr();
    dis_string("Initializing    ", 16);
    comp_pin_d = 0;
    comp_pin = 0;
    Stepper_Init();
    INTCONbits.GIE = 1;
    INTCON2bits.INTEDG0 = 0;        //For External Hardware Interrupt
    INTCONbits.INT0IE = 1;          //For External Hardware Interrupt
}

void main() {
    Init();
    GetReset();
    clr_scr();
    first_line();
    dis_string("Waiting for CMD ",16);
    Open_UART1();
    Open_UART2();
    
    while(1) {
        
        if(adxl_en) {
            ADXL_Read();
            ADXL_Display();
        }
        
        if(int1flag) {
            RxI1();
        }
        
        if(int2flag) {
            RxI2();
        }
        
        if(comp_tmr_en) {
            Compressor_Timer();
        }
        
   }
 
}

void interrupt high_priority high_isr() {
    
    if(PIR1bits.RC1IF) {
        byte1 = RCREG1;
        int1flag = 1;
    }
    
    if(PIR3bits.RC2IF) {
        byte2 = RCREG2;
        int2flag = 1;
        
    }
    
    if(INTCONbits.INT0IF) {
        if(INTCON2bits.INTEDG0 == 0) {
            rstflag = 0;
        }
        INTCONbits.INT0IF = 0;
    }
    
    if(INTCONbits.TMR0IF && comp_tmr_en) {
        Reload_Timer();
        second_inc();
        INTCONbits.TMR0IF = 0;
    }
    
}